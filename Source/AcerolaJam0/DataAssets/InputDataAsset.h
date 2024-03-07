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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Turn")
	float CameraTurnSensitivity = 1.0f;

	/**
	 * Camera position interpolation delay in seconds
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Turn")
	float CameraInterpolationDelay = 0.3f;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move To Point")
	float MaxForceLDistance = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move To Point")
	float GoingUpBoostAngle = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move To Point")
	float GoingUpBoostScale = 1.25f;
	
	/**
	 * Camera position interpolation delay in seconds
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Move To Point")
	float ForcePerSecond = 1000.f;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Parallax")
	float CameraParallaxAmplitude = 100.f;
	
};
