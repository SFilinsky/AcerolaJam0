// Fill out your copyright notice in the Description page of Project Settings.


#include "EventQueueSubsystem.h"

void UEventQueueSubsystem::ExecuteEvents()
{
	ExecuteCategoryQueue(VFX);
	ExecuteCategoryQueue(SFX);
	ExecuteCategoryQueue(Physics);

	ClearAllQueues();
}

void UEventQueueSubsystem::EnqueueEvent(const EEventCategory Category, const TFunction<void()>& Event)
{
	auto& EventQueue = EventQueueMap.FindOrAdd(Category);
	EventQueue.Add(Event);
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


