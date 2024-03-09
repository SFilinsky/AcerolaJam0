// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FACActorRewind.generated.h"

USTRUCT()
struct FActorStateSnapshot
{
	GENERATED_BODY()

	float Time;
	FVector Position;
	FQuat Rotation;
	FVector Velocity;
	FVector AngularVelocity;
};

/**
 * 
 */
UCLASS()
class FEATURE_ACTORREWINDRUNTIME_API UFACActorRewind : public UGKActorComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	FActorStateSnapshot* GetSnapshot(USceneComponent* Component, const float CurrentTime);
	void LoadSnapshot(USceneComponent* Component, const float CurrentTime);

	
	/**
	 * Finds snapshots by time interval
	 *
	 * Will work correctly only on sorted state snapshots array, but will work fast
	 */
	void GetBeforeAfterIndexes(float CurrentTime, int32& OutBefore, int32& OutAfter);
	void RemoveAllSnapshotsAfterTime(float CurrentTime);

	/* State */
	
protected:

	UPROPERTY()
	USceneComponent* MovedComponent;

	TArray<FActorStateSnapshot*> StateSnapshots;
};
