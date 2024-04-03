// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKTimerManager.h"
#include "Subsystems/WorldSubsystem.h"
#include "GKWorldTimerSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE(FRealTimerDynamicDelegate);

/**
 * Allows to use Real Time timers bound to current World.
 *
 * Use for Timers that should be cleaned on Level transitions.
 *
 * Is intended to work in Editor worlds too.
 */
UCLASS()
class GAMEPLAYKIT_API UGKWorldTimerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	/* Overrides */
	
	UGKWorldTimerSubsystem();

	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override;
	virtual bool IsTickableWhenPaused() const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	/* API */

public:

	UFUNCTION(BlueprintCallable)
	const FGKTimerHandle SetTimer(const FRealTimerDynamicDelegate Event, float Interval, bool bLoop = false, float InitialDelay = 0.0f);
	
	UFUNCTION(BlueprintCallable)
	void PauseTimer(const FGKTimerHandle& TimerHandle);
	
	UFUNCTION(BlueprintCallable)
	void ResumeTimer(const FGKTimerHandle& TimerHandle);
	
	UFUNCTION(BlueprintCallable)
	void ClearTimer(const FGKTimerHandle& TimerHandle);


	FGKTimerHandle SetTimer(const TFunction<void()>& Callback, float Interval, bool bLoop = false, float InitialDelay = 0.0f);
	void SetTimerClearCondition(const FGKTimerHandle& TimerHandle, const TFunction<bool()>& Callback);
	/* State */

protected:

	UPROPERTY()
	UGKTimerManager* RealTimeTimerManager;

	float PreviousRealTime = -1;
};
