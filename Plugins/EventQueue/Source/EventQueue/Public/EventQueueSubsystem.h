// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AcerolaJam0/LevelEvents/LevelEventBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "EventQueueSubsystem.generated.h"


/**
 * 
 */
UCLASS()
class EVENTQUEUE_API UEventQueueSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	
public:

	// Called every tick or at a specific interval
	void ExecuteEvents();
	
	/**
	 * Pushes event to specific category queue
	 *
	 * It will be executed when EventQueueSubsystem ticks
	 */
	void EnqueueEvent(const FEventUpdateInformation& EventUpdateInformation);

protected:
	
	void ExecuteCategoryQueue(EEventCategory Category);
	void ClearAllQueues();
	
	/* State */
	
private:
	// Queue to store events
	TMap<EEventCategory, TArray<TFunction<void()>>> EventQueueMap;
	
};
