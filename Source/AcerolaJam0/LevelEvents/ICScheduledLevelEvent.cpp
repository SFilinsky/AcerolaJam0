// Fill out your copyright notice in the Description page of Project Settings.


#include "ICScheduledLevelEvent.h"

TArray<FEventUpdateInformation> UICScheduledLevelEvent::HandleUpdate(const FBeatInfo CurrentBeatInfo)
{
	auto Handlers = Super::HandleUpdate(CurrentBeatInfo);

	if (CurrentBeatInfo.IsBeatNow(BeatNumber))
	{
		if (CurrentBeatInfo.Direction == Positive)
		{
			Handlers.Add({	
				.Category = Category,
				.Callback = [this]() { this->OnTriggered.Broadcast(); }
			});
		} 
	
		if (CurrentBeatInfo.Direction == Negative)
		{
			Handlers.Add({	
				.Category = Category,
				.Callback = [this]() { this->OnRewind.Broadcast(); }
			});
		}
		
		return Handlers;
	}
	
	if (CurrentBeatInfo.IsBeatNow(BeatNumber - PrepareDurationInBeats))
	{
		if (CurrentBeatInfo.Direction == Positive)
		{
			Handlers.Add({	
				.Category = Category,
				.Callback = [this]() { this->OnPrepare.Broadcast(); }
			});
		} 
	
		if (CurrentBeatInfo.Direction == Negative)
		{
			Handlers.Add({	
				.Category = Category,
				.Callback = [this]() { this->OnPrepareRewind.Broadcast(); }
			});
		}
		
		return Handlers;
	}
	
	return Handlers;
}
