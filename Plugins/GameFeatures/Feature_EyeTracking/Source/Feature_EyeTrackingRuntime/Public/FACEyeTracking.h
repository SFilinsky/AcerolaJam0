// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FACEyeTracking.generated.h"

class UInterpolatorFloat;

/**
 * Make ghost eyes follow cursor position 
 */
UCLASS()
class FEATURE_EYETRACKINGRUNTIME_API UFACEyeTracking : public UGKActorComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UFUNCTION(Blueprintable, BlueprintNativeEvent, CallInEditor)
	void EditorWatch();
	
	UFUNCTION(BlueprintCallable)
	void WatchPosition(FVector NewWatchedPosition);

protected:
	
	USceneComponent* GetEyesComponent();
	FTransform GetTargetTransform(FTransform ActualEyeTransform);
	void UpdateEyeMaterial();
	FRotator GetWatchedRotation(FVector WatchedPositionLS);

	/* State */

protected:

	UPROPERTY(EditDefaultsOnly)
	FName EyesTag = FName("Eyes");

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* EyeMaterial;
	
	UPROPERTY(BlueprintReadWrite)
	float TrackingIntervalSeconds = 0.15f;
	
	UPROPERTY()
	UInterpolatorFloat* RotationInterpolator;
	
	FVector WatchedPosition = FVector::ZeroVector;
};
