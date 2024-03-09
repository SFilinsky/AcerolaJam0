// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FPCTimeTracker.generated.h"

/**
 * 
 */
UCLASS()
class FEATURE_SOUNDTRACKTIMECOUNTERRUNTIME_API UFPCTimeTracker : public UGKActorComponent
{
	GENERATED_BODY()
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeChanged, float, CurrentTime, float, TimeModifier);
	
	UPROPERTY(BlueprintAssignable)
	FOnTimeChanged OnTimeChanged;

};
