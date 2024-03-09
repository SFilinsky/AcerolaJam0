// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LevelTimeChecker.generated.h"

/**
 * Allows to easily check if level is rewinded without messing around with LevelTimeSubsystem
 */
UCLASS()
class LEVELTIME_API ULevelTimeChecker : public UObject
{
	GENERATED_BODY()

public:

	bool IsRewinded();
	bool IsPaused();
	bool IsRewindedOrPaused();
};
