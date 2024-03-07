// Fill out your copyright notice in the Description page of Project Settings.


#include "FACDissapearActor.h"

void UFACDissapearActor::Disappear()
{	
	auto Owner = GetOwner();
	if (Owner == nullptr)
	{
		return;
	}
	
	const auto RootComponent = Owner->GetRootComponent();
	if (RootComponent == nullptr)
	{
		return;
	}
	RootComponent->SetVisibility(false);

	
	auto RootPrimitive = Cast<UPrimitiveComponent>(RootComponent); 
	if (RootPrimitive == nullptr)
	{
		return;
	}
	RootPrimitive->SetSimulatePhysics(false);
	RootPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootPrimitive->SetEnableGravity(false);
}
