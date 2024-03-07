// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceKit/GKInterfaceComponent.h"
#include "ICGameFinished.generated.h"

/**
 * Will be triggered when Game was finished
 */
UCLASS()
class INTERFACE_GAMEFINISHED_API UICGameFinished : public UGKInterfaceComponent
{
	GENERATED_BODY()

public:
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameFinishedDelegate);
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGameFinishedDelegate OnGameFinished;
	
};
