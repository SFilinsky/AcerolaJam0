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
	
	const auto DirectionalAcceleration = GetDirectionalAcceleration(StartPointTransform, Position);
	const auto DistanceRatio = GetAccelerationDistanceRatio(StartPointTransform, Position);
	const auto SideBoostAcceleration = GetSideBoostAcceleration(StartPointTransform, Position, MovedPrimitive->GetPhysicsLinearVelocity());
	
	const auto TotalAcceleration = DistanceRatio * (SideBoostAcceleration + DirectionalAcceleration);
	
	const float SpeedRatio = FMath::Clamp(MovedPrimitive->GetPhysicsLinearVelocity().Length() / SoftSpeedCap, 0.0f, 1.0f);
	const auto TotalAccelerationCapped = TotalAcceleration * (1 - SpeedRatio);


	// DrawDebugLine(MovedPrimitive->GetWorld(),MovedPrimitive->GetComponentLocation(),MovedPrimitive->GetComponentLocation() + TotalAcceleration / 100, FColor::Green,false, 3.0f, 0, 0.75f);
	
	MovedPrimitive->AddForce(TotalAccelerationCapped * MovedPrimitive->GetMass()); // We multiply for mass to get final force value; It will make mass to not impact acceleration at the end.


	// Will soften gravity if target point is above current lowest point of actor
	const auto AntiGravityAcceleration = GetVerticalAdjustmentAcceleration(StartPointTransform, Position);
	MovedPrimitive->AddForce(AntiGravityAcceleration * MovedPrimitive->GetMass());
	
}

FVector UFACMouseMovement::GetDirectionalAcceleration(const FTransform& StartTransform, const FVector& TargetPosition)
{
	const FVector StartPosition = StartTransform.GetLocation();
	const FVector StartUpDirection = StartTransform.GetRotation().GetUpVector();

	FVector Direction = TargetPosition - StartPosition;
	Direction.Normalize();

	FVector HorizontalDirection = FVector::VectorPlaneProject(Direction, StartUpDirection);
	HorizontalDirection.Normalize();
	
	return HorizontalDirection * AccelerationPerSecond;
}


float UFACMouseMovement::GetAccelerationDistanceRatio(const FTransform& StartTransform, const FVector& TargetPosition)
{
	return FMath::Sqrt(0.01 + FMath::SmoothStep<float>(0.0f, MaxAccelerationDistance, (StartTransform.GetLocation() - TargetPosition).Length()));
}

FVector UFACMouseMovement::GetVerticalAdjustmentAcceleration(const FTransform& StartTransform, const FVector& TargetPosition)
{
	const auto World = GetWorld();
	checkSlow(World);

	if (StartTransform.GetLocation().Z < TargetPosition.Z)
	{
		return -1 * GravityAdjustmentRatio * FVector::UpVector * World->GetGravityZ();
	}
	
	return FVector::ZeroVector;
}

FVector UFACMouseMovement::GetSideBoostAcceleration(const FTransform& StartTransform, const FVector& TargetPosition, const FVector& CurrentVelocity)
{
	const auto TargetDirection = TargetPosition - StartTransform.GetLocation();
	const auto DirectionalDifference = GetDirectionalDifference(CurrentVelocity, TargetDirection);
	const auto DirectionBoostRatio = FMath::Lerp(0, MaxDirectionalBoostRatio, DirectionalDifference); 
	return TargetDirection.GetSafeNormal() * CurrentVelocity.Length() * DirectionBoostRatio;
}

float UFACMouseMovement::GetDirectionalDifference(const FVector& VecA, const FVector& VecB)
{
	const FVector NormalizedA = VecA.GetSafeNormal();
	const FVector NormalizedB = VecB.GetSafeNormal();
    
	// Get the dot product, which ranges from -1 to 1
	const float DotProd = FVector::DotProduct(NormalizedA, NormalizedB);

	// Adjust the range from -1 -> 1 to 0 -> 1
	return (1 - DotProd) / 2.0f;
}

