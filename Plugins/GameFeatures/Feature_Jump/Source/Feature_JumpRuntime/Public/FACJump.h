// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FACJump.generated.h"

UCLASS(Blueprintable)
class FEATURE_JUMPRUNTIME_API UFACJump : public UGKActorComponent
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void Jump();

	/* State */

protected:

	UPROPERTY(BlueprintReadWrite)
	float JumpImpulsePerKg = 1000.0f;
	
};
