// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataAsset.generated.h"

/**
 * Data asset to store input balance values 
 */
UCLASS(BlueprintType)
class ACEROLAJAM0_API UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/**
	 * Multiplier for camera turning speed
	 *
	 * Scales linearly
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Orbiting")
	float CameraOrbiting_CameraTurnSensitivity = 1.0f;

	/**
	 * Camera position interpolation delay in seconds
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Orbiting")
	float CameraOrbiting_CameraInterpolationDelay = 0.3f;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move To Point")
	float MoveToPoint_MaxAccelerationDistance = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move To Point")
	float MoveToPoint_GravityAdjustmentRatio = 0.75f;
	
	/**
	 * Camera position interpolation delay in seconds
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move To Point")
	float MoveToPoint_AccelerationPerSecond = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move To Point")
	float MoveToPoint_DirectionalBoostRatio = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move To Point")
	float MoveToPoint_SoftSpeedCap = 300.f;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mouse Parallax")
	float MouseParallax_CameraParallaxAmplitude = 100.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Speed Effects")
	float CameraEffects_MinSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Speed Effects")
	float CameraEffects_MaxSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Speed Effects")
	float CameraEffects_MaxParallax = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Speed Effects")
	float CameraEffects_MaxFOVShift = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Speed Effects")
	float CameraEffects_MaxChromaticAberrationIntensityShift = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Speed Effects")
	float CameraEffects_InterpolationDelay = 0.3f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eye Tracking")
	float EyeTracking_TrackingIntervalSeconds = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float Jump_ImpulsePerKg = 1000.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewind Controls")
	float Rewind_DelayFlat = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewind Controls")
	float Rewind_DelayInBeats = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewind Controls")
	float Rewind_Flat = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewind Controls")
	float Rewind_Ratio = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewind Controls")
	float Rewind_ExpRatio = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewind Controls")
	float Rewind_RecoveryFlat = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rewind Controls")
	float Rewind_RecoveryDelayBeats = 0.15f;

};
