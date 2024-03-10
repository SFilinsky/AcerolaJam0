// Fill out your copyright notice in the Description page of Project Settings.


#include "FGMSoundtrackManager.h"

#include "ICSoundtrackFinished.h"
#include "LevelTimeSubsystem.h"
#include "AcerolaJam0/DataAssets/LevelScheduleDataAsset.h"
#include "AcerolaJam0/Overrides/GMLevelBase.h"
#include "Components/AudioComponent.h"
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
	const auto LoadedRewindSoundtrack = Schedule->BackwardsSoundtrackFile.LoadSynchronous();
	if (!ensure(LoadedSoundtrack != nullptr) || !ensure(LoadedRewindSoundtrack != nullptr))
	{
		DestroyComponent();
		return;
	}
	
	Soundtrack = UGameplayStatics::CreateSound2D(World, LoadedSoundtrack);
	Soundtrack->bAutoDestroy = false;
	RewindSoundtrack = UGameplayStatics::CreateSound2D(World, LoadedRewindSoundtrack);
	RewindSoundtrack->bAutoDestroy = false;
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
	
	const auto CurrentTime = LevelTimeSubsystem->GetCurrentTime();
	const auto CurrentTimeDirection = LevelTimeSubsystem->GetTimeDirection();
	const auto CurrentTimeModifier = LevelTimeSubsystem->GetTimeModifier();

	SwitchPlayedSoundtrack(CurrentTime, CurrentTimeDirection, FMath::Abs(CurrentTimeModifier));

	if (bSoundtrackFinished)
	{
		return;
	} 
	
	const auto Sound = Soundtrack->Sound;
	if (LevelTimeSubsystem->GetCurrentTime() > Sound->Duration)
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

void UFGMSoundtrackManager::SwitchPlayedSoundtrack(float CurrentTime, ETimeDirection CurrentTimeDirection, float CurrentTimeModifier)
{
	
	if (LastTickDirection != CurrentTimeDirection)
	{
		LastTickDirection = CurrentTimeDirection;
		
 		if (CurrentlyPlayedSoundtrack != nullptr)
		{
			StopSoundtrack(CurrentlyPlayedSoundtrack);
			CurrentlyPlayedSoundtrack = nullptr;
		}
		
		if (CurrentTimeDirection == ETimeDirection::Positive)
		{
			CurrentlyPlayedSoundtrack = Soundtrack;
			PlaySoundtrack(Soundtrack, CurrentTime);
		}
		
		if (CurrentTimeDirection == ETimeDirection::Negative)
		{
  			CurrentlyPlayedSoundtrack = RewindSoundtrack;
			const auto CurrentTimeFromEnd = GetCurrentTimeFromEnd(RewindSoundtrack, CurrentTime);
			PlaySoundtrack(RewindSoundtrack, CurrentTimeFromEnd);
		}

		if (CurrentTimeDirection == ETimeDirection::Paused)
		{
			// We don't need anything, current already stopped 
			return;
		}
	}


	if (CurrentlyPlayedSoundtrack != nullptr)
	{
		UpdateSoundtrackSpeed(CurrentlyPlayedSoundtrack, CurrentTimeModifier);
	}
}


void UFGMSoundtrackManager::PlaySoundtrack(UAudioComponent* AudioComponent, float StartTime)
{
	CurrentlyPlayedSoundtrack = AudioComponent;
	AudioComponent->Play(StartTime);
}

void UFGMSoundtrackManager::UpdateSoundtrackSpeed(UAudioComponent* AudioComponent, float PlaybackSpeed)
{
	LastTickTimeModifier = PlaybackSpeed;
	AudioComponent->SetPitchMultiplier(PlaybackSpeed);
}

void UFGMSoundtrackManager::StopSoundtrack(UAudioComponent* AudioComponent)
{
	AudioComponent->Stop();
}

float UFGMSoundtrackManager::GetCurrentTimeFromEnd(UAudioComponent* AudioComponent, float StartTime)
{
	const auto Duration = AudioComponent->Sound->Duration;
	return FMath::Clamp(Duration - StartTime, 0.0f, Duration);
}