// Fill out your copyright notice in the Description page of Project Settings.


#include "Proximity.h"

#include "GameplayKit/HelperFunctions/ForEachArrayElement.h"

void UProximity::SetFirstPointListLambda(FPositionsLambda Lambda)
{
	FirstPointListLambda = Lambda;
}

void UProximity::SetSecondPointListLambda(FPositionsLambda Lambda)
{
	SecondPointListLambda = Lambda;
}


float UProximity::GetLatestDistance() const
{
	if (FirstPointListLambda && SecondPointListLambda)
	{
		TArray<FVector> FirstPointList = FirstPointListLambda();
		TArray<FVector> SecondPointList = SecondPointListLambda();

		float MinDistance = 10000000;
		ForEachArrayElement(FirstPointList, [&MinDistance, &SecondPointList](const FVector FirstPoint)
		{
			ForEachArrayElement(SecondPointList, [&MinDistance, &FirstPoint](const FVector SecondPoint)
			{
				MinDistance = FMath::Min(MinDistance, FVector::Dist(FirstPoint, SecondPoint));
			});
		});
		
		return MinDistance;
	}
    
	// Return a default or invalid value if the lambdas are not set
	return -1.0f;
}
