// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCIntro.h"

#include "LevelTimeSubsystem.h"

void UFPCIntro::StopTime()
{
	const auto World = GetWorld();
	checkSlow(World);

	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	checkSlow(TimeSubsystem);

	TimeSubsystem->SetTimeModifier(0);
}

void UFPCIntro::ResumeTime()
{
	const auto World = GetWorld();
	checkSlow(World);

	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	checkSlow(TimeSubsystem);

	TimeSubsystem->SetTimeModifier(1);
}

void UFPCIntro::NotifyIntroFinished()
{

	
}
