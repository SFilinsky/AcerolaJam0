// Fill out your copyright notice in the Description page of Project Settings.


#include "FGMSoundtrackManager.h"

#include "ICSoundtrackFinished.h"
#include "LevelTimeSubsystem.h"
#include "AcerolaJam0/DataAssets/LevelScheduleDataAsset.h"
#include "AcerolaJam0/Overrides/GMLevelBase.h"
#include "InterfaceKit/Subsystems/InterfaceSubsystem/GKInterfaceSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UFGMSoundtrackManager::BeginPlay()
{
	Super::BeginPlay();

	const auto Owner = GetOwner();
	checkSlow(Owner);

	const auto GMLevel = Cast<AGMLevelBase>(Owner);
	checkSlow(GMLevel);

	Schedule = GMLevel->GetLevelSchedule();
	checkSlow(Schedule);

	const auto World = GetWorld();
	checkSlow(World);

	const auto LoadedSoundtrack = Schedule->SoundtrackFile.LoadSynchronous();
	
	UGameplayStatics::PlaySound2D(World, LoadedSoundtrack);
}

void UFGMSoundtrackManager::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto World = GetWorld();
	checkSlow(World);
	
	const auto GameInstance = World->GetGameInstance();
	checkSlow(GameInstance);
	
	const auto LevelTimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	const auto InterfaceSubsystem = GameInstance->GetSubsystem<UGKInterfaceSubsystem>();

	if (LevelTimeSubsystem->IsPaused())
	{
		return;
	}

	const auto LoadedSoundtrack = Schedule->SoundtrackFile.LoadSynchronous();
	if (!bSoundtrackFinished && LevelTimeSubsystem->GetCurrentTime() > LoadedSoundtrack->Duration)
	{
		bSoundtrackFinished = true;
		
		const auto InterfaceInstances = InterfaceSubsystem->GetInterfaceClassInstances(UICSoundtrackFinished::StaticClass());

		for (const auto InterfaceInstance : InterfaceInstances)
		{
			const auto SoundtrackFinishedInterface = Cast<UICSoundtrackFinished>(InterfaceInstance);
			if (!ensure(SoundtrackFinishedInterface != nullptr))
			{
				continue;
			}

			SoundtrackFinishedInterface->OnSoundtrackFinished.Broadcast();
		}
	}
}
