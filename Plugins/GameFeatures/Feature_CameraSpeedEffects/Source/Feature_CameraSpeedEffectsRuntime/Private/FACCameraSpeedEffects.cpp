// Fill out your copyright notice in the Description page of Project Settings.


#include "FACCameraSpeedEffects.h"

#include "Camera/CameraComponent.h"
#include "GameplayKit/Interpolator/UInterpolatorFloat.h"
#include "GameplayKit/Interpolator/UInterpolatorVector3.h"

void UFACCameraSpeedEffects::BeginPlay()
{
	Super::BeginPlay();

	// Find the camera component
	UpdatedCameraComponent = Cast<UCameraComponent>(GetOwner()->FindComponentByClass<UCameraComponent>());
	if (!ensure(UpdatedCameraComponent != nullptr))
	{
		return;
	}
	
	UpdatedCameraComponent->PostProcessSettings.bOverride_SceneFringeIntensity = true;

	LastTickParallax = NewObject<UInterpolatorVector3>(this);
	LastTickFovShift = NewObject<UInterpolatorFloat>(this);
	LastTickChromaticAberrationShift = NewObject<UInterpolatorFloat>(this);
}

void UFACCameraSpeedEffects::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!UpdatedCameraComponent)
	{
		return;
	}
	
	ReverseCameraValues();
	UpdateEffectTargets();
	InterpolateEffectValues(DeltaTime);
	ApplyCameraValues();
}

void UFACCameraSpeedEffects::UpdateEffectTargets()
{
	const FVector CurrentVelocity = GetOwner()->GetVelocity();
	const float CurrentSpeed = CurrentVelocity.Size();

	// Calculate the velocity ratio using SmoothStep
	const float SpeedRatio = FMath::SmoothStep(MinSpeed, MaxSpeed, CurrentSpeed);

	// Apply effects based on the speed ratio
	const FVector Parallax = -SpeedRatio * MaxParallax * CurrentVelocity.GetSafeNormal2D();

	const float FOVShift = MaxFOV * SpeedRatio;
	
	const float ChromaticAberrationIntensityShift = MaxChromaticAberrationIntensity * SpeedRatio;

	LastTickParallax->SetTargetValue(Parallax);
	LastTickFovShift->SetTargetValue(FOVShift);
	LastTickChromaticAberrationShift->SetTargetValue(ChromaticAberrationIntensityShift);
}

void UFACCameraSpeedEffects::InterpolateEffectValues(const float DeltaTime)
{
	LastTickParallax->Interpolate(DeltaTime);
	LastTickFovShift->Interpolate(DeltaTime);
	LastTickFovShift->Interpolate(DeltaTime);
}

void UFACCameraSpeedEffects::ApplyCameraValues()
{
	UpdatedCameraComponent->AddWorldOffset(LastTickParallax->GetCurrentValue());
	UpdatedCameraComponent->SetFieldOfView(UpdatedCameraComponent->FieldOfView + LastTickFovShift->GetCurrentValue());
	UpdatedCameraComponent->PostProcessSettings.SceneFringeIntensity += LastTickChromaticAberrationShift->GetCurrentValue();
}

void UFACCameraSpeedEffects::ReverseCameraValues()
{
	const auto LastTickParallaxValue = LastTickParallax->GetCurrentValue();
	const auto LastTickFovShiftValue = LastTickFovShift->GetCurrentValue();
	const auto LastTickChromaticAberrationShiftValue = LastTickChromaticAberrationShift->GetCurrentValue();
	
	UpdatedCameraComponent->AddWorldOffset(-1.0f * LastTickParallaxValue);
	UpdatedCameraComponent->SetFieldOfView(UpdatedCameraComponent->FieldOfView - LastTickFovShiftValue);
	UpdatedCameraComponent->PostProcessSettings.SceneFringeIntensity -= LastTickChromaticAberrationShiftValue;
}
