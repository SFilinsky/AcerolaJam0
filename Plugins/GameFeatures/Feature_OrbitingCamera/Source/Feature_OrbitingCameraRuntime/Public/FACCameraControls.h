// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FACCameraControls.generated.h"

class UInterpolatorFloat;

/**
 * 
 */
UCLASS()
class FEATURE_ORBITINGCAMERARUNTIME_API UFACCameraControls : public UGKActorComponent
{
	GENERATED_BODY()

	UFACCameraControls();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	
	/**
	 * Virtually adds turn to camera
	 *
	 * Will to effect during short interpolation delay
	 */
	UFUNCTION(BlueprintCallable, Category="Camera Movement")
	void AddTurn(float AngleDeg);

protected:
	
	/* State */

protected:
	
	UPROPERTY()
	USceneComponent* ControlledComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInterpolatorFloat* Position;

};
