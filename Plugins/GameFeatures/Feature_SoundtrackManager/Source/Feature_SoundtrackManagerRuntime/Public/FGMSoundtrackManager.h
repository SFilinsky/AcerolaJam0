// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FGMSoundtrackManager.generated.h"

class ULevelScheduleDataAsset;

/**
 * Handles game soundtrack flow
 */
UCLASS(Blueprintable)
class FEATURE_SOUNDTRACKMANAGERRUNTIME_API UFGMSoundtrackManager : public UGKActorComponent
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* State */
	
protected:

	bool bSoundtrackFinished = false;
	const ULevelScheduleDataAsset* Schedule;
	
};
