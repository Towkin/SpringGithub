// Fill out your copyright notice in the Description page of Project Settings.

#include "Spring.h"
#include "SpringPawnMovementComponent.h"

USpringPawnMovementComponent::USpringPawnMovementComponent() {

	PrimaryComponentTick.bCanEverTick = true;
	bWantsBeginPlay = true;
	


}

void USpringPawnMovementComponent::BeginPlay() {

	GetOwner()->GetComponents(SpringArray);

	Super::BeginPlay();
}

void USpringPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(10, 2, FColor::Blue, "MovementComponent: Either no owner, not updating or we should skip update.");
		}
		return;
	}

	FString DebugText = "";

	Velocity += NextFrameVelocityAdd;

	FHitResult MoveResult;
	SafeMoveUpdatedComponent(Velocity * DeltaTime, UpdatedComponent->GetComponentRotation(), true, MoveResult);
	
	for (auto& Spring : SpringArray) {
		Spring->SetWorldRotation((-SpringDirectionVector - FVector(0.f, 0.f, DirectionFactorZ)).Rotation());
	}
	SpringDirectionVector = FVector(0.f);
	
	DebugText += "\nVelocity: " + Velocity.ToString();
	if (MoveResult.IsValidBlockingHit()) {
		// Remove velocity in the Normal direciton.
		float BounceFactor = 0.1f;
		Velocity = (Velocity.ProjectOnToNormal(MoveResult.Normal) * FVector(1.f, 1.f, -BounceFactor)).ProjectOnTo(Velocity);
		SlideAlongSurface(Velocity * DeltaTime, 1.f - MoveResult.Time, MoveResult.Normal, MoveResult);
		DebugText += "\nHit: " + MoveResult.ImpactPoint.ToString();
	}

	FVector SpringForceVector = FVector::ZeroVector;

	for (auto& Spring : SpringArray) {
		float SpringForce = Spring->GetSpringForce(Velocity, DeltaTime);
		FVector SpringDir = Spring->GetComponentRotation().Vector();

		SpringForceVector -= SpringDir * SpringForce;

	}
	DebugText += "\nSpringForceVector: " + SpringForceVector.ToString();

	NextFrameVelocityAdd = (SpringForceVector / Mass) + (Gravity);
	NextFrameVelocityAdd *= DeltaTime;

	DebugText += "\nNextFrameVelocityAdd: " + NextFrameVelocityAdd.ToString();
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(10, 2, FColor::Blue, DebugText);
	}
	
}
void USpringPawnMovementComponent::AddInputVector(FVector WorldVector, bool bForce) {
	SpringDirectionVector += WorldVector;
}

void USpringPawnMovementComponent::SetSprint(bool SprintEnabled) {
	if (SprintEnabled) {
		DirectionFactorZ = FactorSprintZ;
	} else {
		DirectionFactorZ = FactorDefaultZ;
	}
}