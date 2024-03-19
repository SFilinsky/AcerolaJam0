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
	if (RewindRecoveryStartTime > -1)
	{
		RewindRecoveryStartTime = -1;
	}

	RewindStartTime = GetWorld()->GetRealTimeSeconds();
}

void UFPCRewindControls::StopRewind()
{
	if (RewindStartTime > -1)
	{
		RewindStartTime = -1;
	}

	RewindRecoveryStartTime = GetWorld()->GetRealTimeSeconds();
}

void UFPCRewindControls::HandleRewind()
{
	if (RewindStartTime == -1)
	{
		return;
	}

	const auto World = GetWorld();
	checkSlow(World);
	
	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();

	const auto TimeNow = World->GetRealTimeSeconds();
	const auto TimePassed = TimeNow - RewindStartTime;

	float TimeMultiplier;
	if (TimePassed < RewindStartDelayInSeconds)
	{
		const auto PartPassed = FMath::Clamp(TimePassed / RewindStartDelayInSeconds, 0, 1);
		TimeMultiplier = FMath::Lerp(1, RewindStartDelayFlatBase,  PartPassed);
	} else {
		TimeMultiplier = -1.0 * (RewindMultiplierBase + FMath::Pow(RewindMultiplierRatio * TimePassed, RewindMultiplierExpRatio));
	}
			
	TimeSubsystem->SetTimeModifier(TimeMultiplier);
}

void UFPCRewindControls::HandleRewindRecovery()
{
	if (RewindRecoveryStartTime == -1)
	{
		return;
	}
	
	const auto World = GetWorld();
    checkSlow(World);

    const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	
	const auto TimeNow = World->GetRealTimeSeconds();
	const auto TimePassed = TimeNow - RewindRecoveryStartTime;
	const auto PartPassed = FMath::Clamp(TimePassed / RewindRecoverySeconds, 0, 1);
			
	const auto TimeModifier = FMath::Lerp(RewindRecoveryFlatBase, 1, PartPassed);
	TimeSubsystem->SetTimeModifier(TimeModifier);

	if (TimePassed >= RewindRecoverySeconds)
	{
		RewindRecoveryStartTime = -1;
	}
}