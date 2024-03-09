// Fill out your copyright notice in the Description page of Project Settings.


#include "FACMouseParallax.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"

void UFACMouseParallax::BeginPlay()
{
	Super::BeginPlay();
}

void UFACMouseParallax::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto World = GetWorld();
	checkSlow(World);
	
	const auto Owner = GetOwner();
	checkSlow(Owner);
	
	auto MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(World) * UWidgetLayoutLibrary::GetViewportScale(World) / UWidgetLayoutLibrary::GetViewportSize(World);
	auto NormalisedMousePosition = MousePosition * 2 - 1;

	const auto Camera = Owner->GetComponentByClass<UCameraComponent>();
	if (Camera == nullptr)
	{
		return;
	}

	// Local Space implementation
	auto NewParallaxCameraSpace = FVector(0, NormalisedMousePosition.X, -1 * NormalisedMousePosition.Y) * ParallaxAmplitude;
	NewParallaxCameraSpace = Camera->GetComponentTransform().TransformVector(NewParallaxCameraSpace);

	// World Space implementation
	const auto LocalZDirection = FVector(0, 0, -1);
	const auto WorldZDirection = Camera->GetComponentTransform().TransformVector(LocalZDirection);
	auto WorldZProjection = FVector::VectorPlaneProject(WorldZDirection, FVector::UpVector);
	WorldZProjection.Normalize();

	
	const auto LocalYDirection = FVector(0, 1, 0);
	const auto WorldYDirection = Camera->GetComponentTransform().TransformVector(LocalYDirection);
	auto WorldYProjection = FVector::VectorPlaneProject(WorldYDirection, FVector::UpVector);
	WorldYProjection.Normalize();
	
	const auto NewParallaxProjected = (WorldZProjection * NormalisedMousePosition.Y + WorldYProjection * NormalisedMousePosition.X) * ParallaxAmplitude;
	
	const auto NewParallaxTotal = 0.75 * NewParallaxProjected + 0.25 * NewParallaxCameraSpace;

	
	Camera->AddWorldOffset(PreviousParallax * -1.0);	
	Camera->AddWorldOffset(NewParallaxTotal);
	
	PreviousParallax = NewParallaxTotal;
}
