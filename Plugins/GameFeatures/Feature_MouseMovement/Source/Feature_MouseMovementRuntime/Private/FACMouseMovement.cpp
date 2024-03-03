// Fill out your copyright notice in the Description page of Project Settings.


#include "FACMouseMovement.h"

#include "Feature_MouseMovementRuntimeModule.h"
#include "Components/BoxComponent.h"

void UFACMouseMovement::MoveToPosition(FVector Position)
{
	const auto Owner = GetOwner();

	if (Owner == nullptr)
	{
		return;
	}

	const auto MovedComponent = Owner->GetRootComponent();
	if (MovedComponent == nullptr)
	{
		UE_LOG(LogMouseMovement, Error, TEXT("%s: Tried to move owner [%s], but it has no root component."), *GetName(), *Owner->GetName());
	}

	const auto MovedPrimitive = Cast<UPrimitiveComponent>(MovedComponent);
	if (MovedPrimitive == nullptr)
	{
		UE_LOG(LogMouseMovement, Error, TEXT("%s: Tried to move owner [%s], but it's root component isn't UPrimitiveComponent."), *GetName(), *Owner->GetName());
	}

	auto StartPointTransform = MovedPrimitive->GetComponentTransform();

	/*
	 * BoxCollider position is in the middle, we want lowest point
	 */
	if (const auto BoxCollider = Cast<UBoxComponent>(MovedComponent))
	{
		StartPointTransform = FTransform(MovedPrimitive->GetComponentTransform());
		StartPointTransform.SetLocation(StartPointTransform.GetLocation() - FVector(0.0f, 0.0f, BoxCollider->GetScaledBoxExtent().Z));
	}
	
	const auto ProjectedDirection = GetForceDirection(StartPointTransform, Position);

	DrawDebugLine(MovedPrimitive->GetWorld(),MovedPrimitive->GetComponentLocation(),MovedPrimitive->GetComponentLocation() + ProjectedDirection * 100, FColor::Green,false, 5.0f, 0, 1.0f);

	UE_LOG(LogMouseMovement, Log, TEXT("%f"), ProjectedDirection.Length());
	
	MovedPrimitive->AddForce(ProjectedDirection * ForcePerSecond);
}

FVector UFACMouseMovement::GetForceDirection(const FTransform& StartTransform, const FVector& TargetPosition)
{
	const FVector StartPosition = StartTransform.GetLocation();
	const FVector StartUpDirection = StartTransform.GetRotation().GetUpVector();

	FVector Direction = TargetPosition - StartPosition;
	Direction.Normalize();

	FVector HorizontalDirection = FVector::VectorPlaneProject(Direction, StartUpDirection);
	HorizontalDirection.Normalize();
	
	bool bIsAbove = (TargetPosition.Z - StartPosition.Z) > 0.5f;
	if (bIsAbove)
	{
		FVector RotationAxis = FVector::CrossProduct(StartUpDirection, HorizontalDirection).GetSafeNormal();
		return HorizontalDirection.RotateAngleAxis(-GoingUpBoostAngle, RotationAxis) / GoingUpBoostScale;
	}
	
	return HorizontalDirection;
	
	// float TargetAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(HorizontalDirection, Direction)));
	//
	// FVector ForceDirection = Direction;
	// if (TargetAngle > 5.f)
	// {
	// 	FVector RotationAxis = FVector::CrossProduct(StartUpDirection, HorizontalDirection).GetSafeNormal();
	//
	// 	ForceDirection = HorizontalDirection.RotateAngleAxis(MaxVerticalAngle, RotationAxis);
	// }
	//
	// return ForceDirection;
}
