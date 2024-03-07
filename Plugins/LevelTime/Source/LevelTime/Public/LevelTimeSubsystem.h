// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelTimeSubsystem.generated.h"

/**
 * Manages level time to allow backwards time flow
 */
UCLASS(Blueprintable)
class LEVELTIME_API ULevelTimeSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

protected:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override
	{
		return GetStatID();
	}

public:

	UFUNCTION(BlueprintCallable, Category="Time Flow")
	void Unpause();
	
	UFUNCTION(BlueprintCallable, Category="Time Flow")
	void Pause();

	UFUNCTION(BlueprintCallable, Category="Time Flow")
	bool IsPaused();
	/**
	 * Will set TimeDilation on World, to time flow is changed for all actors.
	 *
	 * Sign is stored locally and is applied on managed CurrentTime.
	 */
	UFUNCTION(BlueprintCallable, Category="Time Flow")
	void SetTimeModifier(float NewTimeModifier);

	UFUNCTION(BlueprintCallable, Category="Time Flow")
	float GetCurrentTime();

	UFUNCTION(BlueprintCallable, Category="Time Flow")
	float GetAdjustedFrameDelta();

	UFUNCTION(BlueprintCallable, Category="Time Flow")
	float GetTimeDirection();
	
	/* State */
	
protected:

	float CurrentTime = 0.0f;
	float TimeDirection = 1.0f;
};
