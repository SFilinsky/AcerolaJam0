// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GKInterfaceComponent.generated.h"

/**
 * Replacement for Unreal built-in Interface system. It has several advantages:
 * - dynamic (can be added to actor and removed at runtime)
 * - works both for C++ and Blueprints
 * - uses composition (scales better, allows to reuse logic and avoid direct dependencies in actor)
 * - can be extracted into GameFeature
 */
UCLASS(Blueprintable, ClassGroup=(InterfaceKit), meta=(BlueprintSpawnableComponent) )
class INTERFACEKIT_API UGKInterfaceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGKInterfaceComponent();

	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

protected:

	void NotifyRegistreation();
	void NotifyDeregistration();
	
};
