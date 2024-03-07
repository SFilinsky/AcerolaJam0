// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GMLevelBase.generated.h"

class ULevelScheduleDataAsset;
/**
 * Base game mode for all game modes
 *
 * The main difference - it includes reference to LevelScheduleDataAsset.
 */
UCLASS(Blueprintable, Abstract)
class ACEROLAJAM0_API AGMLevelBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	const ULevelScheduleDataAsset* GetLevelSchedule();
	
	/* State */
	
protected:
	
	UPROPERTY(EditDefaultsOnly, Category="Classes")
	ULevelScheduleDataAsset* LevelSchedule;
	
};
