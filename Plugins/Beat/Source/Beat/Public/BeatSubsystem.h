// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AcerolaJam0/DataAssets/LevelScheduleDataAsset.h"
#include "Subsystems/WorldSubsystem.h"
#include "BeatSubsystem.generated.h"

UENUM()
enum EBeatDirection
{
	Positive,
	Paused,
	Negative
};


USTRUCT(Blueprintable)
struct FBeatInterval {

	GENERATED_BODY()

	FBeatInterval()
	{
		Direction = Positive;
		Start = 0;
		End = 0;
	}
	
	FBeatInterval(EBeatDirection DirectionValue, float StartValue, float EndValue)
	{
		Direction = DirectionValue;
		Start = StartValue;
		End = EndValue;
	};

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EBeatDirection> Direction;
	
	UPROPERTY(BlueprintReadOnly)
	float Start;
	
	UPROPERTY(BlueprintReadOnly)
	float End;
};


/**
 * 
 */
UCLASS()
class BEAT_API UBeatSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

public:

	UFUNCTION(BlueprintCallable)
	float TimeToBeat(float TimeInSeconds);
	
	UFUNCTION(BlueprintCallable)
	float TimeDeltaToBeat(float TimeInSeconds);

	UFUNCTION(BlueprintCallable)
	const FBeatInterval GetCurrentBeatInfo();
	

protected:
	
	const ULevelScheduleDataAsset* Schedule;
};
