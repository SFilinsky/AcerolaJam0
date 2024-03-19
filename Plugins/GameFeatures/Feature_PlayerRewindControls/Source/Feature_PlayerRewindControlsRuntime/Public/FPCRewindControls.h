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
	void HandleRewind();
	void HandleRewindRecovery();

	/* State */

protected:

	/**
	 * Rewind start
	 */
	UPROPERTY(BlueprintReadWrite)
	float RewindStartDelayFlatBase = 0.5f;
	
	UPROPERTY(BlueprintReadWrite)
	float RewindStartDelayInSeconds = 0.25f;

	/**
	 * Rewind time multiplier
	 */
	
	UPROPERTY(BlueprintReadWrite)
	float RewindMultiplierExpRatio = 0.5f;
	
	UPROPERTY(BlueprintReadWrite)
	float RewindMultiplierBase = 0.25f;
	
	UPROPERTY(BlueprintReadWrite)
	float RewindMultiplierRatio = 0.25f;
	
	/**
	 * Rewind recovery
	 */ 
	UPROPERTY(BlueprintReadWrite)
	float RewindRecoveryFlatBase = 0.1f;
	
	UPROPERTY(BlueprintReadWrite)
	float RewindRecoverySeconds = 0.25f;

private:
	
	float RewindStartTime = -1;
	float RewindRecoveryStartTime = -1;

	FTimerHandle StartRewindTimerHandle;
	FTimerHandle StopRewindTimerHandle;
	
};
