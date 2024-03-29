// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FPCIntro.generated.h"

UCLASS()
class FEATURE_INTRORUNTIME_API UFPCIntro : public UGKActorComponent
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	void StopTime();
	
	UFUNCTION(BlueprintCallable)
	void ResumeTime();

	
	UFUNCTION(BlueprintCallable)
	void NotifyIntroFinished();
	
};
