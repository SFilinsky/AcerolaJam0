// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FACDissapearActor.generated.h"

/**
 * Makes actor visually and physically disappear
 * 
 * Actor will not be destroyed, so it can be returned later.
 */
UCLASS()
class FEATURE_DISAPPEARACTORRUNTIME_API UFACDissapearActor : public UGKActorComponent
{
	GENERATED_BODY()

	
protected:
	
	UFUNCTION(BlueprintCallable)
	void Disappear();
	
};
