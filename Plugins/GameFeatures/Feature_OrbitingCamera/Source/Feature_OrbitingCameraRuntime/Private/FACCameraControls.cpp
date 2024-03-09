// Fill out your copyright notice in the Description page of Project Settings.


#include "FACCameraControls.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameplayKit/Interpolator/UInterpolatorFloat.h"

UFACCameraControls::UFACCameraControls():
	ControlledComponent(nullptr),
	Position(nullptr)
{
}

void UFACCameraControls::BeginPlay()
{
	
	Position = NewObject<UInterpolatorFloat>(this);
 
	const auto Owner = GetOwner();

	if (Owner != nullptr)
	{
		const auto Pawn = Cast<APawn>(Owner);

		ControlledComponent = Pawn->GetComponentByClass<USpringArmComponent>();
		
		if (ControlledComponent == nullptr)
		{
			ControlledComponent = Pawn->GetComponentByClass<UCameraComponent>();
		}
	}
	
	Super::BeginPlay();
}

void UFACCameraControls::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ControlledComponent == nullptr)
	{
		return;
	}

	auto Rotation = ControlledComponent->GetComponentRotation();
	
	Rotation.Yaw -= Position->GetCurrentValue();
	Position->Interpolate(DeltaTime);
	Rotation.Yaw += Position->GetCurrentValue();
	
	ControlledComponent->SetWorldRotation(Rotation);
}

void UFACCameraControls::AddTurn(float AngleDeg)
{
	Position->AddToTargetValue(AngleDeg);
}
