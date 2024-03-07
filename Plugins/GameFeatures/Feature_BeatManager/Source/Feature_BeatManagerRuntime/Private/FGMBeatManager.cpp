// Fill out your copyright notice in the Description page of Project Settings.


#include "FGMBeatManager.h"

#include "BeatSubsystem.h"
#include "EventQueueSubsystem.h"
#include "AcerolaJam0/LevelEvents/ICScheduledLevelEvent.h"
#include "AcerolaJam0/Overrides/GMLevelBase.h"
#include "InterfaceKit/Subsystems/InterfaceSubsystem/GKInterfaceSubsystem.h"

void UFGMBeatManager::BeginPlay()
{
	Super::BeginPlay();

	const auto Owner = GetOwner();
	checkSlow(Owner);

	const auto LevelGameMode = Cast<AGMLevelBase>(Owner);
	checkSlow(LevelGameMode)
	
	Schedule = LevelGameMode->GetLevelSchedule();
	checkSlow(Schedule);

	SetAsyncPhysicsTickEnabled(true);
}

void UFGMBeatManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TriggerEvents();
}

void UFGMBeatManager::AsyncPhysicsTickComponent(float DeltaTime, float SimTime)
{
	
	Super::AsyncPhysicsTickComponent(DeltaTime, SimTime);

	// TriggerEvents();
}

void UFGMBeatManager::TriggerEvents()
{
	const auto World = GetWorld();
	checkSlow(World);

	const auto GameInstance = World->GetGameInstance();
	checkSlow(GameInstance);
	
	const auto BeatSubsystem = World->GetSubsystem<UBeatSubsystem>();
	const auto EventQueueSubsystem = World->GetSubsystem<UEventQueueSubsystem>();
	const auto InterfaceSubsystem = GameInstance->GetSubsystem<UGKInterfaceSubsystem>();
	
	const auto BeatInfo = BeatSubsystem->GetCurrentBeatInfo();
	if (BeatInfo.Direction == Paused)
	{
		return;
	}
	
	const auto EventInstances = InterfaceSubsystem->GetInterfaceClassInstances(UICScheduledLevelEvent::StaticClass());
	
	for (const auto EventToActivate : EventInstances)
	{
		const auto ScheduledEvent = Cast<UICScheduledLevelEvent>(EventToActivate);
		checkSlow(ScheduledEvent);

		if (IsBeatNow(BeatInfo, ScheduledEvent->BeatNumber))
		{
			if (BeatInfo.Direction == Positive)
			{
				EventQueueSubsystem->EnqueueEvent(
					ScheduledEvent->Category,
					[ScheduledEvent]() { 
						ScheduledEvent->OnTriggered.Broadcast();
					}
				);
			} 

			if (BeatInfo.Direction == Negative)
			{
				EventQueueSubsystem->EnqueueEvent(
					ScheduledEvent->Category,
					[ScheduledEvent]() { 
						ScheduledEvent->OnRewind.Broadcast();
					}
				);
			}
			
			continue;
		}
		
		if (IsBeatNow(BeatInfo, ScheduledEvent->BeatNumber - ScheduledEvent->PrepareDurationInBeats))
		{
			if (BeatInfo.Direction == Positive)
			{
				EventQueueSubsystem->EnqueueEvent(
					ScheduledEvent->Category,
					[ScheduledEvent]() { 
							ScheduledEvent->OnPrepare.Broadcast();
						}
				);
			} 

			if (BeatInfo.Direction == Negative)
			{
				EventQueueSubsystem->EnqueueEvent(
					ScheduledEvent->Category,
					[ScheduledEvent]() { 
							ScheduledEvent->OnPrepareRewind.Broadcast();
						}
				);
			}
			
			continue;
		}
	}

	EventQueueSubsystem->ExecuteEvents();
}


bool UFGMBeatManager::IsBeatNow(FBeatInterval CurrentBeatInfo, float BeatToCheck) const
{
	return CurrentBeatInfo.Start <= BeatToCheck && CurrentBeatInfo.End >= BeatToCheck;
}


