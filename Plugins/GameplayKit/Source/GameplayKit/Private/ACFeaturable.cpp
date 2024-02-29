// Fill out your copyright notice in the Description page of Project Settings.


#include "ACFeaturable.h"

#include "Components/GameFrameworkComponentManager.h"
#include "InterfaceKit/Subsystems/InterfaceSubsystem/GKInterfaceSubsystem.h"

// Sets default values for this component's properties
UACFeaturable::UACFeaturable()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UACFeaturable::BeginPlay()
{
	Super::BeginPlay();

	RegisterActorAsReceiver();
}

void UACFeaturable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UnRegisterActorAsReceiver();
}

void UACFeaturable::RegisterActorAsReceiver()
{
	const UWorld* World = this->GetWorld();
	const UGameInstance* GameInstance = World->GetGameInstance();
	
	const auto GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance);
	GFCM->AddGameFrameworkComponentReceiver(this->GetOwner());

	const auto InterfaceSubsystem = UGameInstance::GetSubsystem<UGKInterfaceSubsystem>(GameInstance);
	InterfaceSubsystem->AddInterfaceReceiver(this->GetOwner());
}

void UACFeaturable::UnRegisterActorAsReceiver()
{
	const UWorld* World = this->GetWorld();
	const UGameInstance* GameInstance = World->GetGameInstance();
	
	const auto GFCM = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance);
	GFCM->RemoveGameFrameworkComponentReceiver(this->GetOwner());

	const auto InterfaceSubsystem = UGameInstance::GetSubsystem<UGKInterfaceSubsystem>(GameInstance);
	InterfaceSubsystem->RemoveInterfaceReceiver(this->GetOwner());
}
