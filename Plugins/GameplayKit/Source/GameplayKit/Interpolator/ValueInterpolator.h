// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ValueInterpolator.generated.h"

/**
 * Lazily interpolates to target value
 *
 * Can be used inside Components where value should be smoothly interpolated towards target
 */
UCLASS()
class GAMEPLAYKIT_API UValueInterpolator : public UObject
{
	GENERATED_BODY()

	UValueInterpolator();
	
public:
	
	// Function to update the current value towards the target value.
	void Interpolate(float DeltaTime);
	
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
	void AddToTargetValue(float TargetChange);
	
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
	void SetTargetValue(float NewValue);
	
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
	float GetTargetValue() const;

	/**
	 * Is used for initialisation. Use carefully since it can cause strange effects
	 * @return 
	 */
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
	void SetCurrentValue(float NewValue);
	
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
	float GetCurrentValue() const;
	
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
	void SetInterpolationInterval(float NewInterpolationInterval);
	
	/* State */
	
protected:

	float CurrentValue = 0.f;

	float TargetValue = CurrentValue;

	float InterpolationDelay = 0.3f;
	
};
