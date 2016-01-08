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
	
	
	DebugText += "\nVelocity: " + Velocity.ToString();
	if (MoveResult.IsValidBlockingHit()) {
		// Remove velocity in the ImpactNormal direciton... 
		//Velocity = Velocity.ProjectOnToNormal(MoveResult.ImpactNormal);
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