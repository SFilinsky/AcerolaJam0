// Fill out your copyright notice in the Description page of Project Settings.


#include "FACJump.h"

void UFACJump::Jump()
{
	const auto Owner = GetOwner();
	checkSlow(Owner);

	const auto Root = Owner->GetRootComponent();
	checkSlow(Owner);

	const auto PrimitiveComponent = Cast<UPrimitiveComponent>(Root);
	if (!ensure(PrimitiveComponent != nullptr))
	{
		return;
	}

	PrimitiveComponent->AddImpulse(
		PrimitiveComponent->GetComponentTransform().GetRotation().GetUpVector() * JumpImpulsePerKg * PrimitiveComponent->GetMass()
	);
}
