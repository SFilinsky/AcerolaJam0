// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceKit/GKInterfaceComponent.h"
#include "ICDeathable.generated.h"



UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiedDelegate);


UCLASS(Blueprintable, ClassGroup=(Interfaces), meta=(BlueprintSpawnableComponent) )
class INTERFACEKIT_API UICDeathable : public UGKInterfaceComponent
{
	GENERATED_BODY()
	
public:

    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Dying")
    FDiedDelegate OnDied;

	
};
