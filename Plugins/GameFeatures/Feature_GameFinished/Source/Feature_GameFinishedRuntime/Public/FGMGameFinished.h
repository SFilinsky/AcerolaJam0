// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "GameplayKit/Proximity/Proximity.h"
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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoseStateChanged);
	
protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Lose();

	UFUNCTION(BlueprintCallable)
	void UnLose();
	
	UFUNCTION(BlueprintCallable)
	void Win();

public:

	UPROPERTY(BlueprintAssignable)
	FLoseStateChanged OnLose;
	
	UPROPERTY(BlueprintAssignable)
	FLoseStateChanged OnUnLose;
	
	UPROPERTY(BlueprintAssignable)
	FLoseStateChanged OnWin;
	
protected:

	/* State */

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeStopIntervalOnLose = 1.0f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GoalActorType;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayerActorType;
	
	UPROPERTY(EditDefaultsOnly)
	float WinDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GameCloseDelayAfterWin = 15.0f;

	FTimerHandle LoseTimerHandle;
	FTimerHandle WinTimerHandle;

	UPROPERTY()
	UProximity* ProximityDetector;

	UPROPERTY(BlueprintReadOnly)
	bool bWon;
	
};
