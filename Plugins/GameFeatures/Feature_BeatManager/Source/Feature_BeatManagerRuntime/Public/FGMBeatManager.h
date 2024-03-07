// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeatSubsystem.h"
#include "GKActorComponent.h"
#include "AcerolaJam0/DataAssets/LevelScheduleDataAsset.h"
#include "FGMBeatManager.generated.h"

struct FBeatManagerEventChangeInfo;


/**
 * Plays events based on level schedule
 *
 * It will get schedule from GMLevelBase and activate events at defined beats
 */
UCLASS(Blueprintable)
class FEATURE_BEATMANAGERRUNTIME_API UFGMBeatManager : public UGKActorComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;
	
	void TriggerEvents();

protected:
	
	bool IsBeatNow(FBeatInterval CurrentBeatInfo, float BeatToCheck) const;
	
	/* State */

	
protected:
	
	const ULevelScheduleDataAsset* Schedule;
	
};
