// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceKit/GKInterfaceComponent.h"
#include "ICTriggerMove.generated.h"

class USCPositionConfigurator;
class UICRepeatedLevelEvent;

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class FEATURE_MOVEACTORRUNTIME_API UICTriggerMove : public UGKInterfaceComponent
{
	GENERATED_BODY()

	UICTriggerMove();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMoveTargetChangedDelegate,
		float, PreviousTime,
		FTransform, PreviousTransform,
		float, NextTime,
		FTransform, NextTransform
	);

protected:
	
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	
	int32 GetCurrentPositionIndex(int32 ActivationNumber, const TArray<FTransform>& Positions);
	
	/* State */ 

public:

	UPROPERTY(BlueprintAssignable)
	FMoveTargetChangedDelegate OnMoveTargetChanged;
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Position Settings")
	USCPositionConfigurator* PositionConfigurator;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Event Settings")
	UICRepeatedLevelEvent* LevelEvent;
	
};
