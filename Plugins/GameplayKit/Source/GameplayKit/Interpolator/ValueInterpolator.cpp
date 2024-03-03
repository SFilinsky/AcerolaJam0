// Fill out your copyright notice in the Description page of Project Settings.


#include "ValueInterpolator.h"


UValueInterpolator::UValueInterpolator() {}

void UValueInterpolator::Interpolate(float DeltaTime)
{
	const auto Alpha = FMath::Clamp(DeltaTime / InterpolationDelay, 0, 1);
	CurrentValue = FMath::Lerp(CurrentValue, TargetValue, Alpha);
}

void UValueInterpolator::AddToTargetValue(float TargetChange)
{
	TargetValue += TargetChange;
}

void UValueInterpolator::SetTargetValue(float NewTargetValue)
{
	TargetValue = NewTargetValue;
}

float UValueInterpolator::GetTargetValue() const
{
	return TargetValue;
}

void UValueInterpolator::SetCurrentValue(float NewValue)
{
	CurrentValue = NewValue;
}

float UValueInterpolator::GetCurrentValue() const
{
	return CurrentValue;
}

void UValueInterpolator::SetInterpolationInterval(float NewInterpolationInterval)
{
	InterpolationDelay = NewInterpolationInterval;
}
