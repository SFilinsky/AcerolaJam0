// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceKit/GKInterfaceComponent.h"
#include "ICSoundtrackFinished.generated.h"

/**
 * 
 */
UCLASS()
class INTERFACE_SOUNDTRACKFINISHEDRUNTIME_API UICSoundtrackFinished : public UGKInterfaceComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundtrackFinishedDelegate);
	
public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FSoundtrackFinishedDelegate OnSoundtrackFinished;
	
};
