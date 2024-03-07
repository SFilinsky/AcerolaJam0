// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FACCameraParallax.generated.h"

UCLASS(Blueprintable)
class FEATURE_CAMERAPARALLAXRUNTIME_API UFACCameraParallax : public UGKActorComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* State */
	
protected:

	UPROPERTY(BlueprintReadWrite)
	float ParallaxAmplitude = 10.0f;
	
	FVector PreviousParallax = FVector::Zero();
	
};
