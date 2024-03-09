// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "LevelTimeSubsystem.h"
#include "FGMSoundtrackManager.generated.h"

class ULevelScheduleDataAsset;

/**
 * Handles game soundtrack flow
 */
UCLASS(Blueprintable)
class FEATURE_SOUNDTRACKMANAGERRUNTIME_API UFGMSoundtrackManager : public UGKActorComponent
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	void SwitchPlayedSoundtrack(float CurrentTime, ETimeDirection CurrentTimeDirection, float CurrentTimeModifier);

	void PlaySoundtrack(UAudioComponent* AudioComponent, float StartTime);
	void UpdateSoundtrackSpeed(UAudioComponent* AudioComponent, float PlaybackSpeed);
	void StopSoundtrack(UAudioComponent* AudioComponent);
	float GetCurrentTimeFromEnd(UAudioComponent* AudioComponent, float StartTime);

	/* State */
	
protected:

	bool bSoundtrackFinished = false;
	
	ETimeDirection LastTickDirection = ETimeDirection::Paused;
	float LastTickTimeModifier = 0;
	
	UPROPERTY()
	const ULevelScheduleDataAsset* Schedule;
	
	UPROPERTY()
	UAudioComponent* Soundtrack;

	UPROPERTY()
	UAudioComponent* RewindSoundtrack;
	
	UPROPERTY()
	UAudioComponent* CurrentlyPlayedSoundtrack;
};
