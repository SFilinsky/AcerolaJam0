// Fill out your copyright notice in the Description page of Project Settings.


#include "FACCameraControls.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

UFACCameraControls::UFACCameraControls():
	ControlledComponent(nullptr),
	Position(nullptr)
{
}

void UFACCameraControls::BeginPlay()
{
	
	Position = NewObject<UValueInterpolator>(this);
 
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

	if (ControlledComponent != nullptr)
	{
		auto CurrentComponentRotation = ControlledComponent->GetRelativeRotation().Yaw;
		Position->SetCurrentValue(CurrentComponentRotation);
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

	Position->Interpolate(DeltaTime);

	auto Rotation = ControlledComponent->GetRelativeRotation();
	Rotation.Yaw = Position->GetCurrentValue();
	ControlledComponent->SetRelativeRotation(Rotation);
}

void UFACCameraControls::AddTurn(float AngleDeg)
{
	Position->AddToTargetValue(AngleDeg);
}
