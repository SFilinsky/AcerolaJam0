// Fill out your copyright notice in the Description page of Project Settings.


#include "GKTimerManager.h"

UGKTimerManager::UGKTimerManager() {}

const FGKTimerHandle UGKTimerManager::SetTimer(const TFunction<void()>& Callback, float Interval, bool bLoop)
{
	FGuid Guid = FGuid::NewGuid();
	uint64 UniqueID = ((uint64)Guid.A << 32) | (uint64)Guid.B;
	
	FGKTimerHandle Handle(UniqueID);

	FGKTimerInfo TimerInfo;
	TimerInfo.Callback = Callback;
	TimerInfo.Interval = Interval;
	TimerInfo.bLoop = bLoop;
	
	TimerInfoMap.Add(Handle, TimerInfo);

	return Handle;
}

void UGKTimerManager::PauseTimer(const FGKTimerHandle& TimerHandle)
{
	const auto TimerInfo = TimerInfoMap.Find(TimerHandle);
	if (!TimerInfo)
	{
		return;
	}

	TimerInfo->bPaused = true;
}

void UGKTimerManager::ResumeTimer(const FGKTimerHandle& TimerHandle)
{
	const auto TimerInfo = TimerInfoMap.Find(TimerHandle);
	if (!TimerInfo)
	{
		return;
	}
	
	TimerInfo->bPaused = false;
}

void UGKTimerManager::ClearTimer(const FGKTimerHandle& TimerHandle)
{
	TimerInfoMap.Remove(TimerHandle);
}


void UGKTimerManager::SetTimerClearCondition(const FGKTimerHandle Handle, const TFunction<bool()>& ClearCallback)
{	
	const auto TimerInfo = TimerInfoMap.Find(Handle);
	if (!TimerInfo)
	{
		return;
	}

	TimerInfo->ClearCallback = ClearCallback;
}

void UGKTimerManager::SetCurrentTime(float NewCurrentTime)
{
	CurrentTime = NewCurrentTime;
}

void UGKTimerManager::Tick(float DeltaTime)
{
	CurrentTime += DeltaTime;

	for (auto& [TimerHandle, TimerInfo] : TimerInfoMap)
	{
		const bool bShouldBeCleared = TimerInfo.ClearCallback ? TimerInfo.ClearCallback() : false;
		const auto bIsNotValid = !TimerInfo.Callback;
		if (bShouldBeCleared || bIsNotValid)
		{
			ClearTimer(TimerHandle);
			continue;
		}

		if (TimerInfo.bPaused)
		{
			continue;
		}
		
		const int32 TimesToExecute = FMath::RoundToZero(TimerInfo.AccumulatedTime + DeltaTime / TimerInfo.Interval);
		TimerInfo.AccumulatedTime = FMath::Frac((TimerInfo.AccumulatedTime + DeltaTime) / TimerInfo.Interval);

		for (int32 Index = 0; Index < TimesToExecute; Index++)
		{
			TimerInfo.Callback();
		}
	}
}