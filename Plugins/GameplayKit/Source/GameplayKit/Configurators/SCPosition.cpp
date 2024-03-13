// Fill out your copyright notice in the Description page of Project Settings.


#include "SCPosition.h"

USCPosition::USCPosition()
{
}

USCPosition::~USCPosition()
{
	OnMoved.Clear();
}

#if WITH_EDITOR
	void USCPosition::PostEditComponentMove(bool bFinished)
	{
		Super::PostEditComponentMove(bFinished);

		OnMoved.Broadcast();
	}
#endif
