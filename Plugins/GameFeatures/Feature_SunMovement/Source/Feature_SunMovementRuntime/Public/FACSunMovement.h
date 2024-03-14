// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FACSunMovement.generated.h"

class ULevelScheduleDataAsset;
/**
 * Moves sun across the sky based on soundtrack
 */
UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class FEATURE_SUNMOVEMENTRUNTIME_API UFACSunMovement : public UGKActorComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY()
	const ULevelScheduleDataAsset* Schedule;
	
	FRotator CalculateSunRotation(float Progress);
	void UpdateOwnerRotation(FRotator Rotation);

	/* State */

protected:

	#if WITH_EDITORONLY_DATA
		UPROPERTY(EditAnywhere)
		float EditorTimeProgress = 0.5f;
	#endif

	UPROPERTY(EditAnywhere)
	float SunriseElevation = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float SunsetElevation = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float NoonElevation = 90.0f;
	
	UPROPERTY(EditAnywhere)
	float ExpRatio = 3;
	
	UPROPERTY(EditAnywhere)
	float AzimuthRange = 90;
	
	UPROPERTY(EditAnywhere)
	FRotator BaseRotation = FRotator(0.0f, 0.0f, 0.0f);
};
