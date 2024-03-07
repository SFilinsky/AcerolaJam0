// Fill out your copyright notice in the Description page of Project Settings.


#include "GMLevelBase.h"

void AGMLevelBase::BeginPlay()
{
	check(LevelSchedule != nullptr)
	
	Super::BeginPlay();
}

const ULevelScheduleDataAsset* AGMLevelBase::GetLevelSchedule()
{
	return LevelSchedule;
}
