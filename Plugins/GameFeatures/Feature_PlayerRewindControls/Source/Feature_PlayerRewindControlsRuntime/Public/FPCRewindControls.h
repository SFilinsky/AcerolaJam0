// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FPCRewindControls.generated.h"

/**
 * 
 */
UCLASS()
class FEATURE_PLAYERREWINDCONTROLSRUNTIME_API UFPCRewindControls : public UGKActorComponent
{
	GENERATED_BODY()

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION(BlueprintCallable)
	void StartRewind();
	
	UFUNCTION(BlueprintCallable)
	void StopRewind();

	float GetTimeModifier(const float BeatsSinceRewindStart);

protected:

	/* State */

protected:

	// #define REWIND_NOT_STARED -1
	float RewindStartTime = NULL;
	
};
