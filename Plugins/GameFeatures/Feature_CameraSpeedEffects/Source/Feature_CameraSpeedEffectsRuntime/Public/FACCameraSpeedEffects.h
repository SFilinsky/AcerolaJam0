// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FACCameraSpeedEffects.generated.h"

class UInterpolatorFloat;
class UInterpolatorVector3;
class UCameraComponent;

/**
 * Applies effects to actor camera based on speed
 */
UCLASS(Blueprintable)
class FEATURE_CAMERASPEEDEFFECTSRUNTIME_API UFACCameraSpeedEffects : public UGKActorComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	void ApplyCameraValues();
	void ReverseCameraValues();
	
	void UpdateEffectTargets();
	void InterpolateEffectValues(const float DeltaTime);

	/* State */

protected:

	UPROPERTY(BlueprintReadWrite)
	float MinSpeed = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float MaxSpeed = 600.0f;

	UPROPERTY(BlueprintReadWrite)
	float MaxParallax = 100.0f;

	UPROPERTY(BlueprintReadWrite)
	float MaxFOV = 90.0f;

	UPROPERTY(BlueprintReadWrite)
	float MaxChromaticAberrationIntensity = 1.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float InterpolationDelay = 0.3f;

	UPROPERTY()
	UCameraComponent* UpdatedCameraComponent;
	
	// Cache applied values
	UPROPERTY()
	UInterpolatorVector3* LastTickParallax;
	
	UPROPERTY()
	UInterpolatorFloat* LastTickFovShift;
	
	UPROPERTY()
	UInterpolatorFloat* LastTickChromaticAberrationShift;

};
