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

const FBeatInterval UBeatSubsystem::GetCurrentBeatInfo()
{
	const auto World = GetWorld();
	checkSlow(World);
	
	const auto LevelTimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();

	const auto FrameDelta = LevelTimeSubsystem->GetAdjustedFrameDelta();
	const auto LevelTime = LevelTimeSubsystem->GetCurrentTime();
	const auto BeatDelta = TimeDeltaToBeat(FrameDelta);
	const auto CurrentBeat = TimeToBeat(LevelTime);

	if (LevelTimeSubsystem->IsPaused())
	{
		return FBeatInterval(Paused, CurrentBeat, CurrentBeat);
	}

	if (BeatDelta >= 0)
	{
		return FBeatInterval(Positive, CurrentBeat, CurrentBeat + BeatDelta);
	}

	return FBeatInterval(Negative, CurrentBeat + BeatDelta, CurrentBeat);
}
