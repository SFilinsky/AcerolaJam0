// Fill out your copyright notice in the Description page of Project Settings.


#include "FGMGameFinished.h"

// #if WITH_EDITOR
// 	#include "Editor.h"
// #endif

#include "ICGameFinished.h"
#include "ICSoundtrackFinished.h"
#include "LevelTimeSubsystem.h"
#include "AcerolaJam0/Overrides/GMLevelBase.h"
#include "Editor/EditorEngine.h"
#include "GameplayKit/HelperFunctions/ForEachArrayElement.h"
#include "GameplayKit/HelperFunctions/MapArray.h"
#include "InterfaceKit/Subsystems/InterfaceSubsystem/GKInterfaceSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UFGMGameFinished::BeginPlay()
{
	Super::BeginPlay();
	
	const auto World = GetWorld();
	checkSlow(World);
	
	const auto Owner = GetOwner();
	checkSlow(Owner);

	/* Setup time tracking for lose */

	const auto SoundtrackFinishedInterface = Owner->GetComponentByClass<UICSoundtrackFinished>();
	if (!ensure(SoundtrackFinishedInterface != nullptr))
	{
		return;		
	}
	
	SoundtrackFinishedInterface->OnSoundtrackFinished.AddUniqueDynamic(this, &UFGMGameFinished::Lose);
	SoundtrackFinishedInterface->OnSoundtrackUnFinished.AddUniqueDynamic(this, &UFGMGameFinished::UnLose);
	
	/* Setup win distance detectors */
#
	ProximityDetector = NewObject<UProximity>(this);

	ProximityDetector->SetFirstPointListLambda([World, this]()
	    {
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(World, this->GoalActorType, Actors);

			return MapArray(Actors, [](const AActor* Actor)
			{
				return Actor->GetActorLocation();
			});
	    });
	
    ProximityDetector->SetSecondPointListLambda([World, this]()
    	{
    		TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(World, this->PlayerActorType, Actors);

			return MapArray(Actors, [](const AActor*  Actor)
			{
				return Actor->GetActorLocation();
			});
    	});
}

void UFGMGameFinished::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bWon)
	{
		return;
	}
	
	const auto CurrentDistance = ProximityDetector->GetLatestDistance();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("%f"), CurrentDistance));
	}
	
	if (CurrentDistance < WinDistance)
	{
		Win();
	}
}

void UFGMGameFinished::Lose()
{
	const auto World = GetWorld();
	checkSlow(World);

	const auto LevelTimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	checkSlow(LevelTimeSubsystem);

	LevelTimeSubsystem->Pause();

	OnLose.Broadcast();
}


void UFGMGameFinished::UnLose()
{
	/**
	 * We expect that game was unpaused and time modifier set by some other feature
	 */
	
	OnUnLose.Broadcast();
}

void UFGMGameFinished::Win()
{

	bWon = true;
	
	const auto World = GetWorld();
	checkSlow(World);

	const auto GameInstance = World->GetGameInstance();
	checkSlow(GameInstance)

	const auto InterfaceSubsystem = GameInstance->GetSubsystem<UGKInterfaceSubsystem>();
	checkSlow(InterfaceSubsystem);
	
	const auto LevelTimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	checkSlow(LevelTimeSubsystem);

	const auto Instances = InterfaceSubsystem->GetInterfaceClassInstances(UICGameFinished::StaticClass());
	for (const auto Instance : Instances)
	{
		const auto GameFinishedInstance = Cast<UICGameFinished>(Instance);
		checkSlow(Instance);

		GameFinishedInstance->OnGameFinished.Broadcast(GameCloseDelayAfterWin);
	}

	World->GetTimerManager().SetTimer(
		TimerHandle,
		[World, this]()
		{
			UKismetSystemLibrary::QuitGame(World, UGameplayStatics::GetPlayerController(World, 0), EQuitPreference::Quit, true);
		},
	GameCloseDelayAfterWin,
	false
	);

	// LevelTimeSubsystem->Pause();
	
	OnWin.Broadcast();
}
