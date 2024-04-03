// Fill out your copyright notice in the Description page of Project Settings.


#include "GKTimerManager.h"

UGKTimerManager::UGKTimerManager() {}

const FGKTimerHandle UGKTimerManager::SetTimer(const TFunction<void()>& Callback, float Interval, bool bLoop, float InitialDelay)
{
	FGuid Guid = FGuid::NewGuid();
	uint64 UniqueID = ((uint64)Guid.A << 32) | (uint64)Guid.B;
	
	FGKTimerHandle Handle(UniqueID);

	FGKTimerInfo TimerInfo; 
	TimerInfo.Callback = Callback;
	TimerInfo.Interval = Interval;
	TimerInfo.bLoop = bLoop;
	TimerInfo.InitialDelay = InitialDelay;
	TimerInfo.bInitialDelayPassed = InitialDelay == 0;
	
	TimerInfoMap.Add(Handle, TimerInfo);

	return Handle;
}

void UGKTimerManager::PauseTimer(const FGKTimerHandle& TimerHandle)
{
	checkSlow(TimerHandle != NULL);
	
	const auto TimerInfo = TimerInfoMap.Find(TimerHandle);
	if (!TimerInfo)
	{
		return;
	}

	TimerInfo->bPaused = true;
}

void UGKTimerManager::ResumeTimer(const FGKTimerHandle& TimerHandle)
{
	checkSlow(TimerHandle != NULL);
	
	const auto TimerInfo = TimerInfoMap.Find(TimerHandle);
	if (!TimerInfo)
	{
		return;
	}
	
	TimerInfo->bPaused = false;
}

void UGKTimerManager::ClearTimer(const FGKTimerHandle& TimerHandle)
{
	checkSlow(TimerHandle != NULL);
	
	TimerInfoMap.Remove(TimerHandle);
}

void UGKTimerManager::SetTimerClearCondition(const FGKTimerHandle Handle, const TFunction<bool()>& ClearCallback)
{
	checkSlow(TimerHandle != NULL);
	
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
	
	for (auto It = TimerInfoMap.CreateIterator(); It; ++It) 	
	{
		auto& TimerHandle = It.Key();
		auto& TimerInfo = It.Value();
		
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
		
		TimerInfo.AccumulatedTime += DeltaTime;

		int32 TimesToExecute = 0;

		if (!TimerInfo.bInitialDelayPassed)
		{
			if (TimerInfo.AccumulatedTime >= TimerInfo.InitialDelay)
			{
				TimerInfo.AccumulatedTime -= TimerInfo.InitialDelay;
				TimerInfo.bInitialDelayPassed = true;
				TimesToExecute++;
			}
		}

		/**
		 * Only consider intervals if initial delay passed (in the same tick too)
		 */
		if (TimerInfo.bInitialDelayPassed)
		{
			const int32 IntervalsPassed = FMath::RoundToZero(TimerInfo.AccumulatedTime / TimerInfo.Interval);
			TimerInfo.AccumulatedTime = FMath::Fmod(TimerInfo.AccumulatedTime, TimerInfo.Interval);
			TimesToExecute += TimerInfo.bLoop ? IntervalsPassed : FMath::Min(1, IntervalsPassed);
		}
		
		for (int32 Index = 0; Index < TimesToExecute; Index++)
		{
			if (TimerInfo.Callback)
			{
				TimerInfo.Callback();
			}
		}

		if (!TimerInfo.bLoop && TimesToExecute > 0)
		{
			ClearTimer(TimerHandle);
		}
	}
}