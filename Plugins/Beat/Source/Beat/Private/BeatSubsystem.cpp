// Fill out your copyright notice in the Description page of Project Settings.


#include "BeatSubsystem.h"

#include "LevelTimeSubsystem.h"
#include "AcerolaJam0/DataAssets/LevelScheduleDataAsset.h"
#include "AcerolaJam0/Overrides/GMLevelBase.h"


void UBeatSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const auto GameMode = InWorld.GetAuthGameMode();
	checkSlow(GameMode);

	const auto LevelGameMode = Cast<AGMLevelBase>(GameMode);
	checkSlow(LevelGameMode)
	
	Schedule = LevelGameMode->GetLevelSchedule();
	checkSlow(Schedule);

}

float UBeatSubsystem::TimeDeltaToBeat(float TimeInSeconds)
{
	checkSlow(Schedule);
	
	
	return TimeInSeconds * (Schedule->Tempo / 60.0f);
}

float UBeatSubsystem::TimeToBeat(float TimeInSeconds)
{
	checkSlow(Schedule);
	
	
	return TimeInSeconds * (Schedule->Tempo / 60.0f) + 1;
}

const FBeatInfo UBeatSubsystem::GetCurrentBeatInfo()
{
	const auto World = GetWorld();
	checkSlow(World);
	
	const auto LevelTimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();

	
	const auto TimeDirection = LevelTimeSubsystem->GetTimeDirection();
	const auto TimeModifier = LevelTimeSubsystem->GetTimeModifier();
	
	const auto CurrentTime = LevelTimeSubsystem->GetCurrentTime();
	const auto CurrentBeat = TimeToBeat(CurrentTime);
	
	const auto FrameDelta = LevelTimeSubsystem->GetAdjustedFrameDelta();
	const auto BeatDelta = TimeDeltaToBeat(FrameDelta);
	
	return FBeatInfo(
		TimeDirection,
		TimeModifier,
		FMath::Min(CurrentBeat, CurrentBeat + BeatDelta),
		FMath::Max(CurrentBeat, CurrentBeat + BeatDelta),
		CurrentTime
	);
}
