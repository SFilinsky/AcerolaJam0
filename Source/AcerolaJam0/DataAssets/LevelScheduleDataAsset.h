// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelScheduleDataAsset.generated.h"

class UICScheduledLevelEvent;

/**
 * Holds settings for level and schedule of events
 */
UCLASS(BlueprintType)
class ACEROLAJAM0_API ULevelScheduleDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Settings")
	TSoftObjectPtr<UWorld> LevelToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Settings")
	TSoftObjectPtr<USoundBase> SoundtrackFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Settings")
	TSoftObjectPtr<USoundBase> BackwardsSoundtrackFile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Settings")
	float Tempo;
};
