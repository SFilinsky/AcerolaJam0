// Fill out your copyright notice in the Description page of Project Settings.


#include "EventQueueSubsystem.h"

#include "AcerolaJam0/LevelEvents/LevelEventBase.h"

void UEventQueueSubsystem::ExecuteEvents()
{
	ExecuteCategoryQueue(EEventCategory::VFX);
	ExecuteCategoryQueue(EEventCategory::SFX);
	ExecuteCategoryQueue(EEventCategory::Physics);

	ClearAllQueues();
}

void UEventQueueSubsystem::EnqueueEvent(const FEventUpdateInformation& EventUpdateInformation)
{
	auto& EventQueue = EventQueueMap.FindOrAdd(EventUpdateInformation.Category);
	EventQueue.Add(EventUpdateInformation.Callback);
}

void UEventQueueSubsystem::ExecuteCategoryQueue(const EEventCategory Category)
{
	const auto EventQueue = EventQueueMap.FindOrAdd(Category);
	for (const auto Event : EventQueue)
	{
		Event();
	}
}

void UEventQueueSubsystem::ClearAllQueues()
{
	for (auto& Queue : EventQueueMap)
	{
		Queue.Value.Empty();
	}
}


