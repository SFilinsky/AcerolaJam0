// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelTimeChecker.h"
#include "LevelTimeSubsystem.h"

bool ULevelTimeChecker::IsRewinded()
{
	const auto World = GetWorld();
	checkSlow(World);

	const auto LevelTimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	checkSlow(LevelTimeSubsystem);
	
	return LevelTimeSubsystem->GetTimeDirection() == ETimeDirection::Negative;
}

bool ULevelTimeChecker::IsPaused()
{
	const auto World = GetWorld();
	checkSlow(World);

	const auto LevelTimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	checkSlow(LevelTimeSubsystem);
	
	return LevelTimeSubsystem->GetTimeDirection() == ETimeDirection::Paused;
}

bool ULevelTimeChecker::IsRewindedOrPaused()
{
	const auto World = GetWorld();
    checkSlow(World);

    const auto LevelTimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
    checkSlow(LevelTimeSubsystem);
    
    return LevelTimeSubsystem->GetTimeDirection() == ETimeDirection::Negative
		|| LevelTimeSubsystem->GetTimeDirection() == ETimeDirection::Paused;
}
