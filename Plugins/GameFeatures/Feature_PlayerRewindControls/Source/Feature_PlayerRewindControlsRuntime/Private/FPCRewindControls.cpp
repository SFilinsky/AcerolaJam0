// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCRewindControls.h"

#include "LevelTimeSubsystem.h"
#include "GameplayKit/Timers/GKWorldTimerSubsystem.h"

void UFPCRewindControls::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFPCRewindControls::StartRewind()
{
	const auto WorldTimerSubsystem = GetWorld()->GetSubsystem<UGKWorldTimerSubsystem>();
	
	if (HandleRewindRecoveryTimer != NULL)
	{
		WorldTimerSubsystem->ClearTimer(HandleRewindRecoveryTimer);
		HandleRewindRecoveryTimer = NULL;
	}

	const auto RewindStartTime = GetWorld()->GetRealTimeSeconds();
	HandleRewindStartTimer = WorldTimerSubsystem->SetTimer(
		[ RewindStartTime, this ]()
		{
			const auto World = GetWorld();
			
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
		},
		0.016,
		true
	);
}

void UFPCRewindControls::StopRewind()
{
	const auto WorldTimerSubsystem = GetWorld()->GetSubsystem<UGKWorldTimerSubsystem>();
	
	if (HandleRewindStartTimer != NULL)
	{
		WorldTimerSubsystem->ClearTimer(HandleRewindStartTimer);
		HandleRewindStartTimer = NULL;
	}

	const auto RecoveryStartTime = GetWorld()->GetRealTimeSeconds();
	HandleRewindRecoveryTimer = WorldTimerSubsystem->SetTimer(
		[ RecoveryStartTime, this ]()
		{
			const auto World = GetWorld();
			
			const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	
			const auto TimeNow = World->GetRealTimeSeconds();
			const auto TimePassed = TimeNow - RecoveryStartTime;
			const auto PartPassed = FMath::Clamp(TimePassed / RewindRecoverySeconds, 0, 1);
					
			const auto TimeModifier = FMath::Lerp(RewindRecoveryFlatBase, 1, PartPassed);
			TimeSubsystem->SetTimeModifier(TimeModifier);

			if (TimePassed >= RewindRecoverySeconds)
			{
				const auto WorldTimerSubsystem = GetWorld()->GetSubsystem<UGKWorldTimerSubsystem>();
				WorldTimerSubsystem->ClearTimer(HandleRewindRecoveryTimer);
				HandleRewindRecoveryTimer = NULL;
			}
		},
		0.016,
		true
	);
}