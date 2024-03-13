// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelTimeSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "BeatSubsystem.generated.h"

class ULevelScheduleDataAsset;

USTRUCT(Blueprintable)
struct FBeatInfo {

	GENERATED_BODY()

	FBeatInfo()
	{
		Direction = ETimeDirection::Paused;
		TimeModifierSigned = 0;
		Start = 0;
		End = 0;
	}
	
	FBeatInfo(ETimeDirection DirectionValue, float TimeModifierSignedValue, float StartValue, float EndValue, float ExactValue)
	{
		Direction = DirectionValue;
		TimeModifierSigned = TimeModifierSignedValue;
		Start = StartValue;
		End = EndValue;
		Exact = ExactValue;
	};
	
	UPROPERTY(BlueprintReadOnly)
	ETimeDirection Direction;
	
	UPROPERTY(BlueprintReadOnly)
	float TimeModifierSigned;
	
	UPROPERTY(BlueprintReadOnly)
	float Start;
	
	UPROPERTY(BlueprintReadOnly)
	float End;

	UPROPERTY(BlueprintReadOnly)
	float Exact;
	
	/**
	 * Check if this BeatInfo includes wanted Beat
	 */
	bool IsBeatNow(const float BeatToCheck) const
	{
		return Start <= BeatToCheck && End >= BeatToCheck;
	}
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
	const FBeatInfo GetCurrentBeatInfo();
	

protected:

	UPROPERTY()
	const ULevelScheduleDataAsset* Schedule;
};
