// Fill out your copyright notice in the Description page of Project Settings.


#include "FGMClouds.h"

#include "BeatSubsystem.h"
#include "AcerolaJam0/Overrides/GMLevelBase.h"
#include "GameplayKit/HelperFunctions/ForEachArrayElement.h"
#include "Kismet/GameplayStatics.h"

void UFGMClouds::BeginPlay()
{
	Super::BeginPlay();

	const auto Owner = GetOwner();
	checkSlow(Owner);

	const auto GameMode = Cast<AGMLevelBase>(Owner);
	checkSlow(GameMode);
 
	Schedule = GameMode->GetLevelSchedule();

	SpawnObjects();
}

void UFGMClouds::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateObjects();
}

void UFGMClouds::SpawnObjects()
{

	FMath::RandInit(Seed);

	CloudActor = Cast<AActor>(UGameplayStatics::SpawnObject(AActor::StaticClass(), this));
	
	for (uint32 Index = 0; Index < CloudNumber; Index++)
	{
		const FCloudLifetimeInfo CloudInfo = {
			.LifetimeShift = FMath::FRand() * LifetimeCycleDuration,
			.StartPosition = FVector({
				FMath::FRandRange(XMin, XMax),
				FMath::FRandRange(YMin, YMax),
				FMath::FRandRange(ZMin, ZMax),
			}),
			.Speed = MinSpeed * FVector(1.0f, 1.0f, 1.0f) + (Direction.GetSafeNormal() * (MaxSpeed - MinSpeed)),
			.Component = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("Cloud %d"), Index))
		};
		CloudInfo.Component->SetupAttachment(CloudActor->GetRootComponent());
		CloudInfo.Component->RegisterComponent();
		CloudInfo.Component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CloudInfo.Component->SetEnableGravity(false);

		CloudInfo.Component->SetStaticMesh(CloudMesh);
		CloudInfo.Component->SetMaterial(0, UMaterialInstanceDynamic::Create(CloudMaterial, this));
		
		Clouds.Add(CloudInfo);
	}

	UpdateObjects();
}

void UFGMClouds::UpdateObjects()
{
	const auto World = GetWorld();
	checkSlow(World);
	const auto BeatSubsystem = World->GetSubsystem<UBeatSubsystem>();
	checkSlow(BeatSubsystem);
	
	const auto BeatInfo = BeatSubsystem->GetCurrentBeatInfo(); 
	
	ForEachArrayElement(Clouds, [this, BeatInfo](const FCloudLifetimeInfo& CloudInfo)
	{
		const auto CurrentBeat = BeatInfo.Exact;
		const auto Phase = GetCurrentPhase(CurrentBeat, CloudInfo);
		
		const auto Transparency = this->GetTransparencyBasedOnPhase(Phase);
		const auto Material = Cast<UMaterialInstanceDynamic>(CloudInfo.Component->GetMaterial(0));
		Material->SetScalarParameterValue(FName("Transparency"), Transparency);

		const auto Position = GetPositionBasedOnPhase(Phase, CloudInfo);
		CloudInfo.Component->SetWorldLocation(Position);
	});
}

float UFGMClouds::GetCurrentPhase(const float CurrentBeat, const FCloudLifetimeInfo& CloudInfo)
{
	return FMath::Fmod(CurrentBeat + LifetimeCycleDuration - CloudInfo.LifetimeShift, LifetimeCycleDuration) / LifetimeCycleDuration; 
}

float UFGMClouds::GetTransparencyBasedOnPhase(const float Phase)
{
	const auto PhaseFolded = FMath::Abs(Phase - 0.5f);
	return 1 - FMath::SmoothStep(0.0f, 0.3f, PhaseFolded);
}

FVector UFGMClouds::GetPositionBasedOnPhase(const float Phase, const FCloudLifetimeInfo& Cloud)
{
	return Cloud.StartPosition + Phase * Cloud.Speed * LifetimeCycleDuration;
}
