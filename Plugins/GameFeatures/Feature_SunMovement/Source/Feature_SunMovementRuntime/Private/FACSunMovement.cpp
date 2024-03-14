// Fill out your copyright notice in the Description page of Project Settings.


#include "FACSunMovement.h"

#include "LevelTimeSubsystem.h"
#include "AcerolaJam0/DataAssets/LevelScheduleDataAsset.h"
#include "AcerolaJam0/Overrides/GMLevelBase.h"

void UFACSunMovement::BeginPlay()
{
	Super::BeginPlay();
	
	const auto World = GetWorld();
	checkSlow(World);
	
	const auto GameMode = World->GetAuthGameMode();
	checkSlow(GameMode);

	const auto LevelGameMode = Cast<AGMLevelBase>(GameMode);
	checkSlow(LevelGameMode);
	
	Schedule = LevelGameMode->GetLevelSchedule();
	checkSlow(Schedule);
}

void UFACSunMovement::OnRegister()
{
	Super::OnRegister();

	

	#if WITH_EDITOR
		const auto Rotation = CalculateSunRotation(EditorTimeProgress);
	#else
		const auto Rotation = CalculateSunRotation(0.0f);
	#endif

	UpdateOwnerRotation(Rotation);
}

void UFACSunMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const auto World = GetWorld();
	checkSlow(World);
	
	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();

	const auto Progress = FMath::Pow(FMath::Clamp(TimeSubsystem->GetCurrentTime() / Schedule->SoundtrackFile->Duration, 0.001, 1), ExpRatio) / 3;
	
	const auto Rotation = CalculateSunRotation(Progress);

	UpdateOwnerRotation(Rotation);
}

FRotator UFACSunMovement::CalculateSunRotation(float Progress)
{
	// Calculate the elevation angle: from 0 at sunrise, to 90 at noon, back to 0 at sunset
	// We use a sine function for a smooth transition of the sun's elevation angle throughout the day
	float ElevationDegrees;

	if (Progress <= 0.5f) {
		// Rising part of the day - scale to [SunriseElevation, 90]
		ElevationDegrees = FMath::Lerp(SunriseElevation, NoonElevation, FMath::Sin(PI * Progress));
	} else {
		// Setting part of the day - scale to [90, SunsetElevation]
		ElevationDegrees = FMath::Lerp(NoonElevation, SunsetElevation, FMath::Sin(PI * Progress));
	}

	// Set the azimuth angle: this can be adjusted depending on your game's specific needs
	// Here, we'll keep it simple with a fixed azimuth, but you could make this dynamic
	float AzimuthDegrees = AzimuthRange; // Example value, adjust based on your game world

	// Create and return the FRotator
	// Pitch is the Elevation, Yaw is the Azimuth, and Roll is set to 0 (not typically used for sun direction)
	return FRotator(-ElevationDegrees, AzimuthDegrees, 0.0f);
}

void UFACSunMovement::UpdateOwnerRotation(FRotator Rotation)
{
	const auto Owner = GetOwner();
	checkSlow(Owner);

	Owner->GetRootComponent()->SetWorldRotation(Rotation);
}
