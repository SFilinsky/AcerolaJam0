// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCTimeTracker.h"

#include "LevelTimeSubsystem.h"

void UFPCTimeTracker::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto World = GetWorld();
	checkSlow(World);

	const auto TimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();
	checkSlow(TimeSubsystem);

	OnTimeChanged.Broadcast(TimeSubsystem->GetCurrentTime(), TimeSubsystem->GetTimeModifier());
}
