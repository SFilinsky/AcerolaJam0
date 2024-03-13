// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Proximity.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYKIT_API UProximity : public UObject
{
	GENERATED_BODY()
	
public:
	using FPositionsLambda = TFunction<TArray<FVector>()>;

	void SetFirstPointListLambda(FPositionsLambda Lambda);
	void SetSecondPointListLambda(FPositionsLambda Lambda);
	
	float GetLatestDistance() const;

private:
	FPositionsLambda FirstPointListLambda;
	FPositionsLambda SecondPointListLambda;
};
