// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTimeSubsystem.h"

#include "Kismet/GameplayStatics.h"

void ULevelTimeSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

}

void ULevelTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void ULevelTimeSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto World = GetWorld();
	checkSlow(World);
	
	if (!World->IsPaused())
	{
		const float AdjustedDeltaTime = DeltaTime * TimeDirection;
		CurrentTime = FMath::Max(0, CurrentTime + AdjustedDeltaTime);
	}
}

void ULevelTimeSubsystem::Unpause()
{
	const auto World = GetWorld();
	checkSlow(World);
	
	UGameplayStatics::SetGamePaused(World, false);
}

void ULevelTimeSubsystem::Pause()
{
	const auto World = GetWorld();
	checkSlow(World);
	
	UGameplayStatics::SetGamePaused(World, true);
}

bool ULevelTimeSubsystem::IsPaused()
{
	const auto World = GetWorld();
	checkSlow(World);
	
	return World->IsPaused();
}

void ULevelTimeSubsystem::SetTimeModifier(float NewTimeModifier)
{
	const auto World = GetWorld();
	checkSlow(World);
	
	World->GetWorldSettings()->SetTimeDilation(FMath::Abs(NewTimeModifier));
	TimeDirection = FMath::Sign(NewTimeModifier);
}

float ULevelTimeSubsystem::GetTimeModifier()
{
	const auto World = GetWorld();
	checkSlow(World);
	
	return TimeDirection * World->GetWorldSettings()->GetEffectiveTimeDilation();
}

float ULevelTimeSubsystem::GetCurrentTime()
{
	return CurrentTime;
}

float ULevelTimeSubsystem::GetAdjustedFrameDelta()
{
	const auto World = GetWorld();
	checkSlow(World);
 
	const auto Delta = World->GetDeltaSeconds();
	return Delta * TimeDirection;
}

ETimeDirection ULevelTimeSubsystem::GetTimeDirection()
{
	if (TimeDirection > 0)
	{
		return ETimeDirection::Positive;
	}

	if (TimeDirection < 0) {
		return ETimeDirection::Negative;
	}

	return ETimeDirection::Paused;
}
