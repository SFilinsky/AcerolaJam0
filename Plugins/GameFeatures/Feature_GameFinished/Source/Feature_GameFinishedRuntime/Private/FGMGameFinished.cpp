// Fill out your copyright notice in the Description page of Project Settings.


#include "FGMGameFinished.h"

#include "ICGameFinished.h"
#include "InterfaceKit/Subsystems/InterfaceSubsystem/GKInterfaceSubsystem.h"

void UFGMGameFinished::OnWin()
{

	const auto World = GetWorld();
	checkSlow(World);

	const auto GameInstance = World->GetGameInstance();
	checkSlow(GameInstance)

	const auto InterfaceSubsystem = GameInstance->GetSubsystem<UGKInterfaceSubsystem>();
	checkSlow(InterfaceSubsystem);

	const auto Instances = InterfaceSubsystem->GetInterfaceClassInstances(UICGameFinished::StaticClass());
	for (const auto Instance : Instances)
	{
		const auto GameFinishedInstance = Cast<UICGameFinished>(Instance);
		checkSlow(Instance);

		GameFinishedInstance->OnGameFinished.Broadcast();
	}
}
