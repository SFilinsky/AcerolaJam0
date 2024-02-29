// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterfaceKit/GKInterfaceComponent.h"
#include "ICTargetable.generated.h"


UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetedDelegate);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnTargetedDelegate);


UCLASS(Blueprintable, ClassGroup=(Interfaces), meta=(BlueprintSpawnableComponent) )
class INTERFACEKIT_API UICTargetable : public UGKInterfaceComponent
{
	
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UICTargetable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Targeting")
	FTargetedDelegate OnTargeted;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Targeting")
	FUnTargetedDelegate OnUnTargeted;
	
};
