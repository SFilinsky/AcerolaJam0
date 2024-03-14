// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKActorComponent.h"
#include "FGMClouds.generated.h"

class ULevelScheduleDataAsset;

USTRUCT()
struct FCloudLifetimeInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	float LifetimeShift;

	UPROPERTY()
	FVector StartPosition;
	
	UPROPERTY()
	FVector Speed;

	UPROPERTY()
	UStaticMeshComponent* Component;
	
};

/**
 * 
 */
UCLASS()
class FEATURE_CLOUDSRUNTIME_API UFGMClouds : public UGKActorComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SpawnObjects();
	void UpdateObjects();
	float GetCurrentPhase(float CurrentBeat, const FCloudLifetimeInfo& CloudInfo);
	float GetTransparencyBasedOnPhase(const float Phase);
	FVector GetPositionBasedOnPhase(const float Phase, const FCloudLifetimeInfo& Cloud);

	/* State */

protected:
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* CloudMesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* CloudMaterial;

	UPROPERTY(EditDefaultsOnly)
	uint32 Seed = 0;
	
	UPROPERTY(EditDefaultsOnly)
	uint32 CloudNumber = 50;
	
	UPROPERTY(EditDefaultsOnly)
	float LifetimeCycleDuration = 100;
	
	UPROPERTY(EditDefaultsOnly)
	FVector Direction;

	UPROPERTY(EditDefaultsOnly)
	float ZMin;

	UPROPERTY(EditDefaultsOnly)
	float ZMax;

	UPROPERTY(EditDefaultsOnly)
	float XMin;

	UPROPERTY(EditDefaultsOnly)
	float XMax;

	UPROPERTY(EditDefaultsOnly)
	float YMin;

	UPROPERTY(EditDefaultsOnly)
	float YMax;

	UPROPERTY(EditDefaultsOnly)
	float ScaleMin;

	UPROPERTY(EditDefaultsOnly)
	float ScaleMax;
	
	UPROPERTY(EditDefaultsOnly)
	float MinSpeed;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed;

	UPROPERTY()
	const ULevelScheduleDataAsset* Schedule;
	
	UPROPERTY()
	AActor* CloudActor;
	
	UPROPERTY()
	TArray<FCloudLifetimeInfo> Clouds;
};
