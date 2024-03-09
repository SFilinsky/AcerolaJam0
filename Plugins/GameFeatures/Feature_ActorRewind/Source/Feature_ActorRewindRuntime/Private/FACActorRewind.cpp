// Fill out your copyright notice in the Description page of Project Settings.


#include "FACActorRewind.h"
#include "LevelTimeSubsystem.h"

void UFACActorRewind::BeginPlay()
{
	Super::BeginPlay();
	
	const auto Owner = GetOwner();
	checkSlow(Owner);

	MovedComponent = Owner->GetRootComponent();
	checkSlow(MovedComponent);
}

void UFACActorRewind::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto World = GetWorld();
	const auto LevelTimeSubsystem = World->GetSubsystem<ULevelTimeSubsystem>();

	const auto TimeDirection = LevelTimeSubsystem->GetTimeDirection();
	const auto CurrentTime = LevelTimeSubsystem->GetCurrentTime();

	if (TimeDirection > 0)
	{
		StateSnapshots.Add(GetSnapshot(MovedComponent, CurrentTime));
	}

	if (TimeDirection < 0)
	{
		LoadSnapshot(MovedComponent, CurrentTime);
		RemoveAllSnapshotsAfterTime(CurrentTime);
	}

	if (TimeDirection == 0)
	{
		// We don't need anything
	}
}

FActorStateSnapshot* UFACActorRewind::GetSnapshot(USceneComponent* Component, const float CurrentTime)
{
	const auto Snapshot = new FActorStateSnapshot();
	
	Snapshot->Position = Component->GetComponentLocation();
	Snapshot->Rotation = Component->GetComponentQuat();

	const auto Primitive = Cast<UPrimitiveComponent>(Component);
	if (Primitive != nullptr)
	{
		Snapshot->Velocity = Primitive->GetPhysicsLinearVelocity();
		Snapshot->AngularVelocity = Primitive->GetPhysicsAngularVelocityInDegrees();
	}

	Snapshot->Time = CurrentTime;
	
	return Snapshot;
}

void UFACActorRewind::LoadSnapshot(USceneComponent* Component, const float CurrentTime)
{
	int32 BeforeSnapshotIndex = INDEX_NONE;
	int32 AfterSnapshotIndex = INDEX_NONE;
		
	GetBeforeAfterIndexes(CurrentTime, BeforeSnapshotIndex, AfterSnapshotIndex);
	
	if (BeforeSnapshotIndex != INDEX_NONE && AfterSnapshotIndex != INDEX_NONE)
    {
        // Interpolate between the two states
        const FActorStateSnapshot* BeforeSnapshot = StateSnapshots[BeforeSnapshotIndex];
        const FActorStateSnapshot* AfterSnapshot = StateSnapshots[AfterSnapshotIndex];
        
        // Calculate the interpolation alpha based on the time difference
        float Alpha = (CurrentTime - BeforeSnapshot->Time) / (AfterSnapshot->Time - BeforeSnapshot->Time);
        
        FVector InterpolatedPosition = FMath::Lerp(BeforeSnapshot->Position, AfterSnapshot->Position, Alpha);
        FQuat InterpolatedRotation = FQuat::Slerp(BeforeSnapshot->Rotation, AfterSnapshot->Rotation, Alpha);
        FVector InterpolatedVelocity = FMath::Lerp(BeforeSnapshot->Velocity, AfterSnapshot->Velocity, Alpha);
        FVector InterpolatedAngularVelocity = FMath::Lerp(BeforeSnapshot->AngularVelocity, AfterSnapshot->AngularVelocity, Alpha);
        
        // Apply the interpolated values to the MovedComponent
        MovedComponent->SetWorldLocation(InterpolatedPosition);
        MovedComponent->SetWorldRotation(InterpolatedRotation);
        if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(MovedComponent))
        {
            Primitive->SetPhysicsLinearVelocity(InterpolatedVelocity);
            Primitive->SetPhysicsAngularVelocityInDegrees(InterpolatedAngularVelocity);
        }
    }
    else if (BeforeSnapshotIndex != INDEX_NONE)
    {
        // Apply the state directly without interpolation
        const FActorStateSnapshot* Snapshot = StateSnapshots[BeforeSnapshotIndex];
        MovedComponent->SetWorldLocation(Snapshot->Position);
        MovedComponent->SetWorldRotation(Snapshot->Rotation);
        if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(MovedComponent))
        {
            Primitive->SetPhysicsLinearVelocity(Snapshot->Velocity);
            Primitive->SetPhysicsAngularVelocityInDegrees(Snapshot->AngularVelocity);
        }
    }
    else if (AfterSnapshotIndex != INDEX_NONE)
    {
    	const FActorStateSnapshot* Snapshot = StateSnapshots[AfterSnapshotIndex];
    	MovedComponent->SetWorldLocation(Snapshot->Position);
    	MovedComponent->SetWorldRotation(Snapshot->Rotation);
    	if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(MovedComponent))
    	{
    		Primitive->SetPhysicsLinearVelocity(Snapshot->Velocity);
    		Primitive->SetPhysicsAngularVelocityInDegrees(Snapshot->AngularVelocity);
    	}
    }
};

void UFACActorRewind::GetBeforeAfterIndexes(float CurrentTime, int32& OutBefore, int32& OutAfter)
{
	OutBefore = INDEX_NONE;
	OutAfter = INDEX_NONE;

	// Guard against an empty array
	if (StateSnapshots.IsEmpty())
	{
		return;
	}

	// Start from the end of the array since times are more often close to the end
	for (int32 i = StateSnapshots.Num() - 1; i >= 0; --i)
	{
		if (CurrentTime >= StateSnapshots[i]->Time)
		{
			OutBefore = i; // This is the snapshot before the specified time

			// Check if there's a snapshot after the current time
			if (i + 1 < StateSnapshots.Num())
			{
				OutAfter = i + 1; // This is the snapshot after the specified time
			}
			
			break;
		}
	}
}

void UFACActorRewind::RemoveAllSnapshotsAfterTime(float CurrentTime)
{
	StateSnapshots.RemoveAll([CurrentTime](const FActorStateSnapshot* Snapshot) {
		return Snapshot->Time > CurrentTime;
	});
}
