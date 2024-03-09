// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TInterpolator.generated.h"

UCLASS(BlueprintType, ClassGroup = (GameplayKit))
class GAMEPLAYKIT_API UInterpolatorBase : public UObject
{
	GENERATED_BODY()
};

/**
 * Lazily interpolates to target value
 *
 * Can be used inside Components where value should be smoothly interpolated towards target
 */
template<typename ValueType>
class GAMEPLAYKIT_API TInterpolator
{

public:
	
	TInterpolator() {};
	
public:
	
	// Function to update the current value towards the target value.
	void Interpolate(float DeltaTime)
	{
		const auto Alpha = FMath::Clamp(DeltaTime / InterpolationDelay, 0, 1);
		CurrentValue = FMath::Lerp(CurrentValue, TargetValue, Alpha);
	}
	
	void AddToTargetValue(ValueType TargetChange)
	{
		TargetValue += TargetChange;
	}

	
	void SetTargetValue(ValueType NewTargetValue)
	{
		TargetValue = NewTargetValue;
	}
	
	ValueType GetTargetValue() const
	{
		return TargetValue;
	}

	/**
	 * Is used for initialisation. Use carefully since it can cause strange effects
	 * @return 
	 */
	void SetCurrentValue(ValueType NewValue)
	{
		CurrentValue = NewValue;
	}
	
	ValueType GetCurrentValue() const
	{
		return CurrentValue;
	}
	
	void SetInterpolationInterval(float NewInterpolationInterval)
	{
		InterpolationDelay = NewInterpolationInterval;
	}
	
	/* State */
	
protected:

	ValueType CurrentValue;
	ValueType TargetValue;

	float InterpolationDelay = 0.3f;
	
};
