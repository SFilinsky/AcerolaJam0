// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCRewindControls.h"

#include "BeatSubsystem.h"
#include "LevelTimeSubsystem.h"

void UFPCRewindControls::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleRewind();
	HandleRewindRecovery();
}

void UFPCRewindControls::StartRewind()
{
	if (RewindRecoveryStartTime != NULL)
	{
		RewindRecoveryStartTime = NULL;
	}

	RewindStartTime = GetWorld()->GetTimeSeconds();
}

void UFPCRewindControls::StopRewind()
{
	if (RewindStartTime != NULL)
	{
		RewindStartTime = NULL;
	}

	RewindRecoveryStartTime = GetWorld()->GetTimeSeconds();
}

void UFPCRewindControls::HandleRewind()
{
	if (RewindStartTime == NULL)
	{
		return;
	}

	const auto World = GetWorld();
	checkSlow(World);
	
	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	const auto BeatSubsystem = World->GetSubsystem<UBeatSubsystem>();

	const auto TimeNow = World->GetRealTimeSeconds();
	const auto TimePassed = TimeNow - RewindStartTime;
	const auto BeatsPassed = BeatSubsystem->TimeDeltaToBeat(TimePassed);

	float TimeMultiplier;
	if (BeatsPassed < RewindStartDelayInBeats)
	{
		const auto PartPassed = FMath::Clamp(BeatsPassed / RewindStartDelayInBeats, 0, 1);
		TimeMultiplier =  FMath::Lerp(1, RewindStartDelayFlatBase,  PartPassed);
	} else {
		TimeMultiplier = -1.0 * (RewindMultiplierBase + FMath::Pow(RewindMultiplierRatio * BeatsPassed, RewindMultiplierExpRatio));
	}
			
	TimeSubsystem->SetTimeModifier(TimeMultiplier);
}

void UFPCRewindControls::HandleRewindRecovery()
{
	if (RewindRecoveryStartTime == NULL)
	{
		return;
	}
	
	const auto World = GetWorld();
    checkSlow(World);

    const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
    const auto BeatSubsystem = World->GetSubsystem<UBeatSubsystem>();
	
	const auto TimeNow = World->GetRealTimeSeconds();
	const auto TimePassed = TimeNow - RewindRecoveryStartTime;
	const auto BeatsPassed = BeatSubsystem->TimeDeltaToBeat(TimePassed);
	const auto PartPassed = FMath::Clamp(BeatsPassed / RewindRecoveryBeats, 0, 1);
			
	const auto TimeModifier = FMath::Lerp(RewindRecoveryFlatBase, 1, PartPassed);
	TimeSubsystem->SetTimeModifier(TimeModifier);

	if (BeatsPassed >= RewindRecoveryBeats)
	{
		RewindRecoveryStartTime = NULL;
	}
}