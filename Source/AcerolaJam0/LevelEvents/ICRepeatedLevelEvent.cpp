// Fill out your copyright notice in the Description page of Project Settings.


#include "ICRepeatedLevelEvent.h"

TArray<FEventUpdateInformation> UICRepeatedLevelEvent::HandleUpdate(const FBeatInfo CurrentBeatInfo)
{
	auto Handlers = Super::HandleUpdate(CurrentBeatInfo);

	const int32 ActivationsBefore = FMath::Floor((CurrentBeatInfo.Start - InitialBeat) / RepeatIntervalInBeats) + 1; 
	const auto CurrentActivationBeat = InitialBeat + ActivationsBefore * RepeatIntervalInBeats;
	
	if (CurrentBeatInfo.IsBeatNow(CurrentActivationBeat))
	{
		
		if (CurrentBeatInfo.Direction == ETimeDirection::Positive)
		{
			const auto NextActivationIndex = ActivationsBefore;
			const auto NextActivationBeat = CurrentActivationBeat + RepeatIntervalInBeats;
			
			Handlers.Add({
				.Category = EEventCategory::Physics,
				.Callback = [this, NextActivationIndex, NextActivationBeat]()
				{
					this->OnTriggered.Broadcast(
						NextActivationIndex,
						NextActivationBeat
					);
				}
			});
		}
	
		if (CurrentBeatInfo.Direction == ETimeDirection::Negative)
		{
			const auto NextActivationIndex = ActivationsBefore - 1;
			const auto NextActivationBeat = CurrentActivationBeat - RepeatIntervalInBeats;
		
			Handlers.Add({
				.Category = EEventCategory::Physics,
				.Callback = [this, NextActivationIndex, NextActivationBeat]()
				{
					this->OnRewind.Broadcast(
						NextActivationIndex,
						NextActivationBeat
					);
				}
			});
		}
	}
	
	return Handlers;
}
