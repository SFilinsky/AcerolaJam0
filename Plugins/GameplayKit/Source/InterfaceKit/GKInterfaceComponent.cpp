// Fill out your copyright notice in the Description page of Project Settings.


#include "GKInterfaceComponent.h"

#include "Subsystems/InterfaceSubsystem/GKInterfaceSubsystem.h"

// Sets default values for this component's properties
UGKInterfaceComponent::UGKInterfaceComponent()
{

	// ...
}

void UGKInterfaceComponent::OnRegister()
{
	Super::OnRegister();

	NotifyRegistreation();
}

void UGKInterfaceComponent::OnUnregister()
{
	Super::OnUnregister();

	NotifyDeregistration();
}

void UGKInterfaceComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);

	if (IsRegistered())
	{
		NotifyDeregistration();
	}
}

void UGKInterfaceComponent::NotifyRegistreation()
{
	const auto World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	
	const auto GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
	{
		return;
	}

	if (const auto InterfaceSubsystem = UGameInstance::GetSubsystem<UGKInterfaceSubsystem>(GameInstance))
	{
		InterfaceSubsystem->NotifyInterfaceAddition(GetOwner(), this->GetClass());
	}
}

void UGKInterfaceComponent::NotifyDeregistration()
{
	const auto World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	
	const auto GameInstance = World->GetGameInstance();
	if (GameInstance == nullptr)
	{
		return;
	}

	if (const auto InterfaceSubsystem = UGameInstance::GetSubsystem<UGKInterfaceSubsystem>(GameInstance))
	{
		InterfaceSubsystem->NotifyInterfaceRemoval(GetOwner(), this->GetClass());
	}
}
