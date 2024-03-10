// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelEventBase.h"
#include "ICScheduledLevelEvent.generated.h"

/**
 * Basis for all level events
 *
 * To create new event, you should extend this class.
 * This is needed for trackability - it can be hard to find places where tag from schedule is applied,
 * but class usages are tracked easily.
 *
 * One more advantage - it will protect from typos in LevelSchedule asset configurations.
 */
UCLASS(Blueprintable, Abstract)
class ACEROLAJAM0_API UICScheduledLevelEvent : public ULevelEventBase
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggerScheduledLevelEventDelegate);

public:

	virtual TArray<FEventUpdateInformation> HandleUpdate(const FBeatInfo CurrentBeatInfo) override;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FTriggerScheduledLevelEventDelegate OnPrepare;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FTriggerScheduledLevelEventDelegate OnTriggered;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FTriggerScheduledLevelEventDelegate OnRewind;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FTriggerScheduledLevelEventDelegate OnPrepareRewind;
	
	/* State */

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	TEnumAsByte<EEventCategory> Category = Physics;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float PrepareDurationInBeats = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	float BeatNumber = 10000.0f	;
	
};
