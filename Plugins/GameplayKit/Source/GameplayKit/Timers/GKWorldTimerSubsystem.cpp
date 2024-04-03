// Fill out your copyright notice in the Description page of Project Settings.


#include "GKWorldTimerSubsystem.h"

UGKWorldTimerSubsystem::UGKWorldTimerSubsystem()
{
	RealTimeTimerManager = CreateDefaultSubobject<UGKTimerManager>(TEXT("RealTimeTimerManager"), false);
}

bool UGKWorldTimerSubsystem::IsTickable() const
{
	return IsInitialized();
}

bool UGKWorldTimerSubsystem::IsTickableInEditor() const
{
	return IsInitialized();
}

bool UGKWorldTimerSubsystem::IsTickableWhenPaused() const
{
	return IsInitialized();
}

void UGKWorldTimerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const auto World = GetWorld();
	checkSlow(World);

	PreviousRealTime = World->GetRealTimeSeconds();
}

void UGKWorldTimerSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto World = GetWorld();
	checkSlow(World);

	const auto CurrentRealTime = World->GetRealTimeSeconds();
	const auto DeltaRealTime = CurrentRealTime - PreviousRealTime; 
	PreviousRealTime = CurrentRealTime;

	RealTimeTimerManager->Tick(DeltaRealTime);
}

TStatId UGKWorldTimerSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UGKWorldTimerSubsystem, STATGROUP_Tickables);
}

const FGKTimerHandle UGKWorldTimerSubsystem::SetTimer(const FRealTimerDynamicDelegate Event, float Interval, bool bLoop, float InitialDelay)
{
	if (!Event.IsBound())
	{
		return FGKTimerHandle();
	}
	
	return SetTimer(
		[Event]()
		{
			if (Event.IsBound())
			{
				Event.Execute();
			}
		},
		Interval,
		bLoop,
		InitialDelay
	);
}

void UGKWorldTimerSubsystem::PauseTimer(const FGKTimerHandle& TimerHandle)
{
	RealTimeTimerManager->PauseTimer(TimerHandle);
}

void UGKWorldTimerSubsystem::ResumeTimer(const FGKTimerHandle& TimerHandle)
{
	RealTimeTimerManager->ResumeTimer(TimerHandle);
}

void UGKWorldTimerSubsystem::ClearTimer(const FGKTimerHandle& TimerHandle)
{
	RealTimeTimerManager->ClearTimer(TimerHandle);
}

FGKTimerHandle UGKWorldTimerSubsystem::SetTimer(const TFunction<void()>& Callback, float Interval, bool bLoop, float InitialDelay)
{	
	return RealTimeTimerManager->SetTimer(Callback, Interval, bLoop, InitialDelay);
}

void UGKWorldTimerSubsystem::SetTimerClearCondition(const FGKTimerHandle& TimerHandle,
                                                    const TFunction<bool()>& Callback)
{
	RealTimeTimerManager->SetTimerClearCondition(TimerHandle, Callback);
}
