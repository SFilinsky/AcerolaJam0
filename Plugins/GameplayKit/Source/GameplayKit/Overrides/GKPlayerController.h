// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GKPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class GAMEPLAYKIT_API AGKPlayerController : public APlayerController
{
	GENERATED_BODY()

public: 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPossessionChanged, APawn*, NewPawn);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPossessionChanged OnPossessionChanged;
	
protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
};
