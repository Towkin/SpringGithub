// Fill out your copyright notice in the Description page of Project Settings.

#include "Spring.h"
#include "SpringPawnMovementComponent.h"

USpringPawnMovementComponent::USpringPawnMovementComponent() {

	PrimaryComponentTick.bCanEverTick = true;
	bWantsBeginPlay = true;
	


}

void USpringPawnMovementComponent::BeginPlay() {

	GetOwner()->GetComponents(SpringArray);

	FixedDeltaTime = 1.f / FixedFrameRate;

	Super::BeginPlay();
}

void USpringPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(40, 2, FColor::Blue, "MovementComponent: Either no owner, not updating or we should skip update.");
		}
		return;
	}

	

	for (auto& Spring : SpringArray) {

		FVector NextHitNormal(0.f, 0.f, -1.f);
		FHitResult NextHitTest;

		FVector NextHitStartLocation = Spring->GetComponentLocation();
		FVector NextHitEndLocation = CalculateLocationAtTime(NextHitStartLocation, Velocity, 1.f, Gravity);
		FCollisionObjectQueryParams NextHitParams(ECollisionChannel::ECC_WorldStatic);
		bool NextHit = GetWorld()->LineTraceSingleByObjectType(NextHitTest, NextHitStartLocation, NextHitEndLocation, NextHitParams);

		FColor DrawColor = FColor::Blue;

		if (NextHit) {
			NextHitNormal = NextHitTest.Normal;
			DrawColor = FColor::Red;
		}

		DrawDebugLine(GetWorld(), NextHitStartLocation, NextHitEndLocation, DrawColor, false, 1.0f, (uint8)'\000', 5.f);


		Spring->GroundVector = -NextHitNormal;
		Spring->GroundScalar = DirectionFactorZ;
		Spring->OffsetVector = -SpringDirectionVector;

		//Spring->SetWorldRotation(FQuat::Slerp(Spring->GetComponentQuat(), (-SpringDirectionVector - FVector(0.f, 0.f, DirectionFactorZ)).Rotation().Quaternion(), 0.25f).Rotator());
	}

	Accumulator += DeltaTime;
	
	FVector OldVelocity = Velocity;

	if (FixedDeltaTime <= 0.f) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(30, 2, FColor::Red, "Invalid FixedDeltaTime: " + FString::SanitizeFloat(FixedDeltaTime));
		}
		return;
	}
	int FixedUpdates = 0;
	while (Accumulator >= FixedDeltaTime) {
		FixedTick(FixedDeltaTime);
		Accumulator -= FixedDeltaTime;
		if (++FixedUpdates >= MaxUpdatesPerFrame) {
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(20, 2, FColor::Red, "SpringPawn physics simulation reached the maximum update count: " + FString::SanitizeFloat(MaxUpdatesPerFrame));
			}
			break;
		}
	}


	SpringDirectionVector = FVector(0.f);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(15, 2, FColor::Blue, "Ran " + FString::SanitizeFloat(FixedUpdates) + " FixedTicks this frame.");
		GEngine->AddOnScreenDebugMessage(18, 2, FColor::Blue, "Velocity: " + Velocity.ToString() + " (Frame change: " + (Velocity - OldVelocity).ToString() + ")");
	}
	
}
void USpringPawnMovementComponent::AddInputVector(FVector WorldVector, bool bForce) {
	SpringDirectionVector += WorldVector;
}

void USpringPawnMovementComponent::FixedTick(float DeltaTime) {
	
	// Start the physics update by moving the accumulated force from last frame.
	Velocity += NextFrameVelocityAdd;
	NextFrameVelocityAdd = FVector::ZeroVector;

	// Do actor movement
	FHitResult MoveResult;
	SafeMoveUpdatedComponent(Velocity * DeltaTime, UpdatedComponent->GetComponentRotation(), true, MoveResult);
	if (MoveResult.IsValidBlockingHit()) {
		// Remove velocity in the Normal direciton.
		float BounceFactor = 0.1f;
		Velocity = (Velocity.ProjectOnToNormal(MoveResult.Normal) * FVector(1.f, 1.f, -BounceFactor)).ProjectOnTo(Velocity);
		SlideAlongSurface(Velocity * DeltaTime, 1.f - MoveResult.Time, MoveResult.Normal, MoveResult);
	}


	FVector SpringForceVector = FVector::ZeroVector;

	bool GroundedTick = false;

	for (auto& Spring : SpringArray) {
		float SpringForce = Spring->GetSpringForce();
		FVector SpringDir = Spring->GetComponentRotation().Vector();

		SpringForceVector -= SpringDir * SpringForce;

		GEngine->AddOnScreenDebugMessage(6783, 2.f, FColor::Yellow, "ForceVector: " + SpringForceVector.ToString() + ", SpringForce: " + FString::SanitizeFloat(SpringForce));

		GroundedTick = GroundedTick || Spring->Grounded;
	}
	
	NextFrameVelocityAdd = (SpringForceVector / Mass) + (Gravity);
	
	if (!GroundedTick) {
		NextFrameVelocityAdd += SpringDirectionVector.GetClampedToMaxSize(1.f) * (AirMoveForce / Mass);
	}

	NextFrameVelocityAdd *= DeltaTime;
	GEngine->AddOnScreenDebugMessage(32873, 2.f, FColor::Cyan, "VelocityAdd: " + NextFrameVelocityAdd.ToString());
}


void USpringPawnMovementComponent::SetSprint(bool SprintEnabled) {
	if (SprintEnabled) {
		DirectionFactorZ = FactorSprintZ;
	} else {
		DirectionFactorZ = FactorDefaultZ;
	}
}

FVector USpringPawnMovementComponent::CalculateLocationAtTime(FVector StartingLocation, FVector StartingVelocity, float Time, FVector ConstantForce) {
	// PositionOffset = StartingVelocity * Time + (1/2) * ConstantForce * Time^2

	return StartingLocation + (StartingVelocity * Time) + (0.5 * ConstantForce * Time * Time);
}