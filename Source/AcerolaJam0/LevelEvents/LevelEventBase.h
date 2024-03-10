// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceKit/GKInterfaceComponent.h"
#include "BeatSubsystem.h"
#include "LevelEventBase.generated.h"

UENUM(Blueprintable)
enum EEventCategory
{
	SFX,
	VFX,
	Physics
};

USTRUCT()
struct FEventUpdateInformation
{
	GENERATED_BODY()
	
	EEventCategory Category;
	TFunction<void()> Callback;
};

/**
 * Base class for all event interfaces in level
 *
 * It should be extended for customising behaviour.
 * 
 * HandleUpdate() function allows to define any behaviour based on received time information
 */
UCLASS(Abstract)
class ACEROLAJAM0_API ULevelEventBase : public UGKInterfaceComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * Takes time information and returns list of callbacks that will be put into event queue
	 */
	virtual TArray<FEventUpdateInformation> HandleUpdate(const FBeatInfo CurrentBeatInfo);
};
