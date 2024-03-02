// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACSwitchMaterial.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACEROLAJAM0_API UACSwitchMaterial : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UACSwitchMaterial();

protected:
	// Called when the game starts
	UFUNCTION(CallInEditor)
	virtual void BeginPlay() override;
	
	/* State */

public:

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Material_Game;
	
};
