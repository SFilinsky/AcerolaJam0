// Fill out your copyright notice in the Description page of Project Settings.


#include "FACEyeTracking.h"

#include "GameplayKit/Interpolator/UInterpolatorFloat.h"
#include "Kismet/KismetMathLibrary.h"

void UFACEyeTracking::BeginPlay()
{
	Super::BeginPlay();

	RotationInterpolator = NewObject<UInterpolatorFloat>(this);
	RotationInterpolator->SetCurrentValue(0);
	RotationInterpolator->SetCurrentValue(0);
	RotationInterpolator->SetInterpolationInterval(TrackingSpeed);
	
	const auto EyesComponent = GetEyesComponent();
	if (!ensure(EyesComponent != nullptr))
	{
		return;
	}

	TArray<USceneComponent*> EyesList;
	EyesComponent->GetChildrenComponents(false, EyesList);
	
	/** Update eye material */
	const auto RightEyeComponent = Cast<UStaticMeshComponent>(EyesList[0]);
	RightEyeComponent->CreateDynamicMaterialInstance(0, EyeMaterial);

	/* Left eye */
	const auto LeftEyeComponent = Cast<UStaticMeshComponent>(EyesList[1]);
	LeftEyeComponent->CreateDynamicMaterialInstance(0, EyeMaterial);
	
}

void UFACEyeTracking::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const auto EyesComponent = GetEyesComponent();
	if (!ensure(EyesComponent != nullptr))
	{
		return;
	}
	
	EyesComponent->AddRelativeRotation(FRotator(0, -1,0) * RotationInterpolator->GetCurrentValue());
	RotationInterpolator->Interpolate(DeltaTime);
	EyesComponent->AddRelativeRotation(FRotator(0, 1,0) * RotationInterpolator->GetCurrentValue());
	
}

void UFACEyeTracking::EditorWatch_Implementation()
{
	RotationInterpolator = NewObject<UInterpolatorFloat>(this);
	RotationInterpolator->SetCurrentValue(0);
	RotationInterpolator->SetCurrentValue(0);
	RotationInterpolator->SetInterpolationInterval(TrackingSpeed);
	
	const auto EyesComponent = GetEyesComponent();
	if (!ensure(EyesComponent != nullptr))
	{
		return;
	}

	TArray<USceneComponent*> EyesList;
	EyesComponent->GetChildrenComponents(false, EyesList);
	
	/** Update eye material */
	const auto RightEyeComponent = Cast<UStaticMeshComponent>(EyesList[0]);
	RightEyeComponent->CreateDynamicMaterialInstance(0, EyeMaterial);

	/* Left eye */
	const auto LeftEyeComponent = Cast<UStaticMeshComponent>(EyesList[1]);
	LeftEyeComponent->CreateDynamicMaterialInstance(0, EyeMaterial);
}

void UFACEyeTracking::WatchPosition(FVector NewWatchedPosition)
{
	WatchedPosition = NewWatchedPosition;
	
	const auto EyesComponent = GetEyesComponent();
	if (!ensure(EyesComponent != nullptr))
	{
		return;
	}
	
	// We want to get local position relative to final interpolated transform
	const auto TargetTransform = GetTargetTransform(EyesComponent->GetComponentTransform());
	const auto TargetLocalWatchedPosition = TargetTransform.InverseTransformPosition(NewWatchedPosition);
	const auto TargetToWatchedRotationDelta = GetWatchedRotation(TargetLocalWatchedPosition);
	RotationInterpolator->AddToTargetValue(TargetToWatchedRotationDelta.Yaw);

	UpdateEyeMaterial();
}


FRotator UFACEyeTracking::GetWatchedRotation(FVector WatchedPositionLS)
{
	return UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, WatchedPositionLS);
}

USceneComponent* UFACEyeTracking::GetEyesComponent()
{
	const auto Owner = GetOwner();
	checkSlow(Owner);

	const auto AllEyes = Owner->GetComponentsByTag(USceneComponent::StaticClass(), EyesTag);
	if (AllEyes.Num() == 0)
	{
		return nullptr;
	}
	
	return Cast<USceneComponent>(AllEyes[0]);
}

FTransform UFACEyeTracking::GetTargetTransform(const FTransform ActualEyeTransform)
{
	auto TransformCopy = FTransform(ActualEyeTransform);
	const auto InterpolationFinalRotation = TransformCopy.GetRotation().Rotator()
		- FRotator(0, 1,0) * RotationInterpolator->GetCurrentValue()
		+ FRotator(0, 1,0) * RotationInterpolator->GetTargetValue();
	TransformCopy.SetRotation(InterpolationFinalRotation.Quaternion());

	return TransformCopy;
}

void UFACEyeTracking::UpdateEyeMaterial()
{
	const auto EyesComponent = GetEyesComponent();
	if (!ensure(EyesComponent != nullptr))
	{
		return;
	}
	
	TArray<USceneComponent*> EyesList;
	EyesComponent->GetChildrenComponents(false, EyesList);
	
	/** Update eye material */
	
	/* Right eye */
	const auto RightEyeComponent = Cast<UStaticMeshComponent>(EyesList[0]);
	const auto RightEyeTransform = RightEyeComponent->GetComponentTransform();
	const auto RightEyeLocalWatchedPosition = RightEyeTransform.InverseTransformPosition(WatchedPosition);
	const auto RightEyeWatchedRotationDelta = GetWatchedRotation(RightEyeLocalWatchedPosition);
	// RightEyeComponent->SetRelativeRotation(RightEyeWatchedRotationDelta);
	const auto DynamicRightEyeMaterialInstance = Cast<UMaterialInstanceDynamic>(RightEyeComponent->GetMaterial(0));
	DynamicRightEyeMaterialInstance->SetScalarParameterValue("LocalPitch", RightEyeWatchedRotationDelta.Pitch / 360.0f);
	DynamicRightEyeMaterialInstance->SetScalarParameterValue("LocalYaw", RightEyeWatchedRotationDelta.Yaw / 360.0f);

	/* Left eye */
	const auto LeftEyeComponent = Cast<UStaticMeshComponent>(EyesList[1]);
	const auto LeftEyeTransform = LeftEyeComponent->GetComponentTransform();
	const auto LeftEyeLocalWatchedPosition = LeftEyeTransform.InverseTransformPosition(WatchedPosition);
	const auto LeftEyeWatchedRotationDelta = GetWatchedRotation(LeftEyeLocalWatchedPosition);
	// LeftEyeComponent->SetRelativeRotation(LeftEyeWatchedRotationDelta);
	const auto DynamicLeftEyeMaterialInstance = Cast<UMaterialInstanceDynamic>(LeftEyeComponent->GetMaterial(0));
	DynamicLeftEyeMaterialInstance->SetScalarParameterValue("LocalPitch", LeftEyeWatchedRotationDelta.Pitch / 360.0f);
	DynamicLeftEyeMaterialInstance->SetScalarParameterValue("LocalYaw", LeftEyeWatchedRotationDelta.Yaw / 360.0f);

	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("%f %f %f %f"),
	// 		RightEyeWatchedRotationDelta.Pitch,
	// 		RightEyeWatchedRotationDelta.Yaw,
	// 		LeftEyeWatchedRotationDelta.Pitch,
	// 		LeftEyeWatchedRotationDelta.Yaw
	// 	));
	// }
}