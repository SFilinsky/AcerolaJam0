// Fill out your copyright notice in the Description page of Project Settings.


#include "FACMouseMovement.h"

#include "Feature_MouseMovementRuntimeModule.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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
	const auto ForceMagnitude = GetForceDistanceRatio(StartPointTransform, Position);
	const auto AntiGravityForce = GetSlopeAdjustmentForce(StartPointTransform, Position);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *AntiGravityForce.ToString());

	DrawDebugLine(MovedPrimitive->GetWorld(),MovedPrimitive->GetComponentLocation(),MovedPrimitive->GetComponentLocation() + ProjectedDirection * 100, FColor::Green,false, 5.0f, 0, 1.0f);
	
	MovedPrimitive->AddForce(ProjectedDirection * ForcePerSecond * ForceMagnitude + AntiGravityForce * MovedPrimitive->GetMass());
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
}


float UFACMouseMovement::GetForceDistanceRatio(const FTransform& StartTransform, const FVector& TargetPosition)
{
	return FMath::Sqrt(0.01 + FMath::SmoothStep<float>(0.0f, MaxForceDistance, (StartTransform.GetLocation() - TargetPosition).Length()));
}

FVector UFACMouseMovement::GetSlopeAdjustmentForce(const FTransform& StartTransform, const FVector& TargetPosition)
{
	const auto World = GetWorld();
	checkSlow(World);
	
	const FVector StartPosition = StartTransform.GetLocation();

	FHitResult OutHit = FHitResult();
	bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, StartPosition, TargetPosition, ECC_Visibility, FCollisionQueryParams(FName(TEXT("")), false, GetOwner()));

	if (!bHit)
	{
		return FVector::Zero();
	}
	
	const FVector SurfaceNormal = OutHit.ImpactNormal;
	const FVector Gravity = -FVector::UpVector * GetWorld()->GetGravityZ();

	FVector AntiGravityForce = FVector::VectorPlaneProject(Gravity, SurfaceNormal);
	
	return AntiGravityForce;
}

