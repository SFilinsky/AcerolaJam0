// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GKTimerManager.generated.h"

DECLARE_DYNAMIC_DELEGATE(FTimerCallback);
DECLARE_DYNAMIC_DELEGATE(FTimerClearCallback);

USTRUCT(Blueprintable, BlueprintType)
struct FGKTimerHandle
{
	GENERATED_BODY()

	FGKTimerHandle()
		: Handle(0)
	{}

	FGKTimerHandle(int32 NewHandle)
		: Handle(NewHandle)
	{}

	bool operator==(const FGKTimerHandle& Other) const
	{
		return Handle == Other.Handle;
	}

	bool operator!=(const FGKTimerHandle& Other) const
	{
		return Handle != Other.Handle;
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("%llu"), Handle);
	}

	friend uint32 GetTypeHash(const FGKTimerHandle& InHandle)
	{
		return GetTypeHash(InHandle.Handle);
	}

	
	/* State */
	
protected: 
	
	uint64 Handle;
};


USTRUCT()
struct FGKTimerInfo
{
	GENERATED_BODY()

	FGKTimerInfo() {}

public:
	
	TFunction<void()> Callback;
	TFunction<bool()> ClearCallback;
	float Interval = 0;
	bool bLoop = false;

	bool bPaused = false;
	float AccumulatedTime = 0;
};

/**
 * Class to control GameplayKit timers. This is needed to feel the gap of real time timers. 
 * UE5.3 only supported World-time timers.
 *
 * This class will be composited into WorldRealTimerSubsystem and GITimeSubsystem to control it's lifecycle.
 * This manager responsibility is to manage timer lifecycle when it's destroyed, but it doesn't know when it happens.
 *
 * This manager also doesn't know about time space it operates in, it should be Ticked from outside. It allows any
 * grade of precision and any time dilation's needed based on where it's called.
 *
 * It can be accessed in subsystem, or created on Component level.
 */
UCLASS(Blueprintable, BlueprintType)
class GAMEPLAYKIT_API UGKTimerManager : public UObject
{
	GENERATED_BODY()

	UGKTimerManager();
	

public:

	const FGKTimerHandle SetTimer(const TFunction<void()>& Callback, float Interval, bool bLoop);
	void PauseTimer(const FGKTimerHandle& TimerHandle);
	void ResumeTimer(const FGKTimerHandle& TimerHandle);
	void ClearTimer(const FGKTimerHandle& TimerHandle);

	/**
	 * Attach Clear Callback to the timer. It will be executed every time before timer execution.
	 * In case it returns false, timer won't be triggered and will be cleared together with handle.
	 */
	void SetTimerClearCondition(const FGKTimerHandle TimerHandle, const TFunction<bool()>& ClearCallback);

	/**
	 * Will ignore any timer that should be triggered between previous and current time.
	 * Is intended for initialisation only.
	 */
	void SetCurrentTime(float NewCurrentTime);

	/**
	 * Moved TimerManager time DeltaTime seconds ahead.
	 * In case of looping timer, will execute it multiple times if loop is shorted then DeltaTime. 
	 */
	void Tick(float DeltaTime);
	

	/* State */ 

protected:

	TMap<FGKTimerHandle, FGKTimerInfo> TimerInfoMap;
	float CurrentTime = 0;
};
