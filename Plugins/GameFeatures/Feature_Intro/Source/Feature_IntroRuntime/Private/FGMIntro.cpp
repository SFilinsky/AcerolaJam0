// Fill out your copyright notice in the Description page of Project Settings.


#include "FGMIntro.h"

#include "LevelTimeSubsystem.h"

void UFGMIntro::StopTime()
{
	const auto World = GetWorld();
	checkSlow(World);

	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	checkSlow(TimeSubsystem);

	TimeSubsystem->SetTimeModifier(0);
}

void UFGMIntro::ResumeTime()
{
	const auto World = GetWorld();
	checkSlow(World);

	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	checkSlow(TimeSubsystem);

	TimeSubsystem->SetTimeModifier(1);
}

void UFGMIntro::NotifyIntroFinished()
{

	
}
