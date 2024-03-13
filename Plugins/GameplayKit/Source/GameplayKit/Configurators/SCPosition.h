// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SCPosition.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYKIT_API USCPosition : public USceneComponent
{
	GENERATED_BODY()

	USCPosition();
	
	virtual ~USCPosition() override;

protected:
	#if WITH_EDITOR
		virtual void PostEditComponentMove(bool bFinished) override;
	#endif
	
public:
	DECLARE_MULTICAST_DELEGATE(FMovedDelegate);
	FMovedDelegate OnMoved;


};
