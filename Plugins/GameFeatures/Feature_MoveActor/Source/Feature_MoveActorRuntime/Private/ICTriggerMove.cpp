// Fill out your copyright notice in the Description page of Project Settings.


#include "ICTriggerMove.h"

#include "AcerolaJam0/LevelEvents/ICRepeatedLevelEvent.h"
#include "GameplayKit/Configurators/SCPositionConfigurator.h"

UICTriggerMove::UICTriggerMove()
{
}

void UICTriggerMove::OnRegister()
{
	Super::OnRegister();
	
	const auto Owner = GetOwner();
	checkSlow(Owner);
	const auto Root = Owner->GetRootComponent();
	checkSlow(Root);

	auto bChanged = false;
	
	if (!LevelEvent)
	{
		LevelEvent = NewObject<UICRepeatedLevelEvent>(this, FName(this->GetName() + "_LevelEvent"));
		LevelEvent->RegisterComponent();
		bChanged = true;
	}
	
	if (!PositionConfigurator)
	{
		PositionConfigurator = NewObject<USCPositionConfigurator>(this, FName(this->GetName() + "_MovementConfigurator"));
		PositionConfigurator->SetupAttachment(Root);
		PositionConfigurator->RegisterComponent();
		bChanged = true;
	}

	// #if WITH_EDITOR
	// 	if (bChanged)
	// 	{
	// 		Owner->ReregisterAllComponents();
	// 		if (GEngine)
	// 		{
	// 			GEngine->BroadcastLevelActorListChanged();
	// 			if (Owner && !Owner->IsTemplate())
	// 			{
	// 				Owner->MarkPackageDirty();
	// 			}
	// 		}
	// 	}
	// #endif
}

void UICTriggerMove::BeginPlay()
{
	Super::BeginPlay();

	const auto World = GetWorld();
	checkSlow(World);

	const auto BeatSubsystem = World->GetSubsystem<UBeatSubsystem>();

	LevelEvent->OnTriggered.AddWeakLambda(this, [this](
		int32 ActivationNumber,
		float TimeToNextActivation
	){
		const auto Positions = PositionConfigurator->GetConfiguredPositions();
		const auto CurrentPosition = GetCurrentPositionIndex(ActivationNumber, Positions);
		const auto NextNumber = GetCurrentPositionIndex(ActivationNumber + 1, Positions);
		
		// OnMoveTargetChanged.Broadcast();
	});
	
	// LevelEvent->OnRewind.AddLambda([this](int32 ActivationNumber)
	// {
	// 	
	// });
}

int32 UICTriggerMove::GetCurrentPositionIndex(int32 ActivationNumber, const TArray<FTransform>& Positions)
{
	if (Positions.IsEmpty())
	{
		// Handle the case where there are no positions.
		return -1;
	}

	int32 PositionsCount = Positions.Num();

	if (ActivationNumber <= PositionsCount)
	{
		return ActivationNumber - 1;
	}
	
	int32 CycleLength = 2 * PositionsCount - 2; // Forward and backward, excluding the duplicate
	int32 PositionInCycle = (ActivationNumber - 1) % CycleLength;

	// Determine if we are moving forward or backward in the list of positions
	if (PositionInCycle < PositionsCount)
	{
		// Moving forward
		return PositionInCycle;
	}
	else
	{
		// Moving backward, excluding the last item from being the start/end
		return CycleLength - PositionInCycle;
	}
}

