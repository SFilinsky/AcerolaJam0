// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FACMouseMovement.generated.h"

/**
 * 
 */
UCLASS()
class FEATURE_MOUSEMOVEMENTRUNTIME_API UFACMouseMovement : public UGKActorComponent
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintCallable)
	void MoveToPosition(FVector Position);

protected:
	
	FVector GetForceDirection(const FTransform& TargetTransform, const FVector& Direction);

	/* State */

protected:

	UPROPERTY(BlueprintReadWrite)
	float GoingUpBoostAngle = 30.f;
	
	UPROPERTY(BlueprintReadWrite)
	float GoingUpBoostScale = 1.25;
	
	UPROPERTY(BlueprintReadWrite)
	float ForcePerSecond = 1000.f;
	
};
