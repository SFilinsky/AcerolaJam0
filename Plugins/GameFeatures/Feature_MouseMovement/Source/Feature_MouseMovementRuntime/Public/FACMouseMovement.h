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

	FVector GetDirectionalAcceleration(const FTransform& TargetTransform, const FVector& Direction);
	float GetAccelerationDistanceRatio(const FTransform& TargetTransform, const FVector& Direction);
	FVector GetVerticalAdjustmentAcceleration(const FTransform& TargetTransform, const FVector& Direction);
	FVector GetSideBoostAcceleration(const FTransform& StartTransform, const FVector& TargetPosition, const FVector& CurrentVelocity);

	/**
	 * Returns value between 0 and 1
	 * 0 means two vectors go in one direction
	 * 1 means vectors are under angle more then 90
	 *
	 * Is used to turn faster on higher 
	 */
	float GetDirectionalDifference(const FVector& VecA, const FVector& VecB);

	/* State */

protected:


	UPROPERTY(BlueprintReadWrite)
	float MaxAccelerationDistance = 100.0f;
		
	UPROPERTY(BlueprintReadWrite)
	float AccelerationPerSecond = 1000.f;

	UPROPERTY(BlueprintReadWrite)
	float MaxDirectionalBoostRatio = 0.2f;

	UPROPERTY(BlueprintReadWrite)
	float GravityAdjustmentRatio = 0.75f;
};