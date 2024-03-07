// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FGMGameFinished.generated.h"

/**
 * Handles game win / lose rules
 *
 * It's expected that class is extended with BP where actual Win rules can be defined
 */
UCLASS(Blueprintable)
class FEATURE_GAMEFINISHEDRUNTIME_API UFGMGameFinished : public UGKActorComponent
{
	GENERATED_BODY()

protected:
	
	/**
	 * Should be called when level is considered complete
	 */
	UFUNCTION(BlueprintCallable)
	void OnWin();
	
};
