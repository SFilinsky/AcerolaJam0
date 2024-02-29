// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayKitComponentManager.generated.h"

/**
 * Custom Extension to Component Manager
 *
 * Is introduced to allow GameplayKit interface usage via API
 */
UCLASS(Blueprintable, DisplayName = "GameplayKitInterfaceManager")
class GAMEPLAYKIT_API UGameplayKitComponentManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	/**
	 * Subsystem that should allow finding actors with Interfaces on them and track them reactively
	 *
	 * One use case I had is attaching ActorComponent from inside Game Feature to Actors that have interface only 
	 */
	
};
