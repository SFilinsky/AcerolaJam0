// Fill out your copyright notice in the Description page of Project Settings.


#include "GKPlayerController.h"

void AGKPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OnPossessionChanged.Broadcast(InPawn);
}

void AGKPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
	
	OnPossessionChanged.Broadcast(nullptr);
}
