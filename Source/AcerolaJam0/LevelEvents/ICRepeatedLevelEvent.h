// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelEventBase.h"
#include "ICRepeatedLevelEvent.generated.h"

/**
 * Basis for all level events
 *
 * To create new event, you should extend this class.
 * This is needed for trackability - it can be hard to find places where tag from schedule is applied,
 * but class usages are tracked easily.
 *
 * One more advantage - it will protect from typos in LevelSchedule asset configurations.
 */
UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class ACEROLAJAM0_API UICRepeatedLevelEvent : public ULevelEventBase
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_TwoParams(FTriggerRepeatedLevelEventDelegate, int32, float);

public:

	virtual TArray<FEventUpdateInformation> HandleUpdate(const FBeatInfo CurrentBeatInfo) override;
	
	FTriggerRepeatedLevelEventDelegate OnTriggered;
	FTriggerRepeatedLevelEventDelegate OnRewind;
	
	/* State */

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TEnumAsByte<EEventCategory> Category = Physics;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float InitialBeat = 10000.0f;
	
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float RepeatIntervalInBeats = 10000.0f;
	
};
