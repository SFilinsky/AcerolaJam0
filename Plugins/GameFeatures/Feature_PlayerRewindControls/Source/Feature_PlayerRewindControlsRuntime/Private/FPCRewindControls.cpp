// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCRewindControls.h"

#include "BeatSubsystem.h"
#include "LevelTimeSubsystem.h"


void UFPCRewindControls::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (RewindStartTime == NULL)
	{
		return;
	}
	
	const auto World = GetWorld();
	checkSlow(World);

	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	const auto BeatSubsystem = World->GetSubsystem<UBeatSubsystem>();

	const auto TimeSinceRewindStart = World->GetTimeSeconds() - RewindStartTime;
	const auto BeatsSinceRewindStart = BeatSubsystem->TimeDeltaToBeat(TimeSinceRewindStart);
	
	const auto TimeModifier = GetTimeModifier(BeatsSinceRewindStart);
	UE_LOG(LogTemp, Warning, TEXT("%f"), TimeModifier);
	
  	TimeSubsystem->SetTimeModifier(TimeModifier);
}

void UFPCRewindControls::StartRewind()
{
	const auto World = GetWorld();
    checkSlow(World);

	RewindStartTime = World->GetTimeSeconds();
}

void UFPCRewindControls::StopRewind()
{
	RewindStartTime = NULL;

	const auto World = GetWorld();
	checkSlow(World);

	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	TimeSubsystem->SetTimeModifier(1);
}

float UFPCRewindControls::GetTimeModifier(const float BeatsSinceRewindStart)
{
	return -1.0;
	
	if (BeatsSinceRewindStart <= 0)
	{
		return 0.0;
	}
	
	return -1.0 * (0.25 + FMath::Pow(BeatsSinceRewindStart, 0.3));
}
