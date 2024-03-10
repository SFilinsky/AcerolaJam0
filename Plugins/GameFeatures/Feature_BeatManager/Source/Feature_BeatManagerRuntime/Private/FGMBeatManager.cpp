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
}

void UFGMBeatManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TriggerEvents();
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
	
	const auto EventInstances = InterfaceSubsystem->GetInterfaceClassInstances(ULevelEventBase::StaticClass());
	for (const auto EventToActivate : EventInstances)
	{
		const auto LevelEvent = Cast<ULevelEventBase>(EventToActivate);
		checkSlow(LevelEvent);

		const auto EventUpdateInformationList = LevelEvent->HandleUpdate(BeatInfo);
		for (const auto EventUpdateInformation : EventUpdateInformationList)
		{
			EventQueueSubsystem->EnqueueEvent(EventUpdateInformation);
		}
	}

	EventQueueSubsystem->ExecuteEvents();
}

