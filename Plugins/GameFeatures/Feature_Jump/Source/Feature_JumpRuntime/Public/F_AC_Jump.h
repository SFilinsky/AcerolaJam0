// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "F_AC_Jump.generated.h"

UCLASS(Blueprintable)
class FEATURE_JUMPRUNTIME_API UF_AC_Jump : public UGKActorComponent
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void Jump();
	
};
