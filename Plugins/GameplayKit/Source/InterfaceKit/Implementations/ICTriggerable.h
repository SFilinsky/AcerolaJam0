// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceKit/GKInterfaceComponent.h"
#include "ICTriggerable.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTriggeredDelegate);

/**
 * Allows to trigger different events indirectly
 */
UCLASS(Blueprintable, ClassGroup=(Interfaces), meta=(BlueprintSpawnableComponent))
class INTERFACEKIT_API UICTriggerable : public UGKInterfaceComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Triggering")
	FTriggeredDelegate OnTriggered;
	
};
