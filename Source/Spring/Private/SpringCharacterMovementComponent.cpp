// Fill out your copyright notice in the Description page of Project Settings.

#include "Spring.h"
#include "SpringCharacterMovementComponent.h"


// Sets default values for this component's properties
USpringCharacterMovementComponent::USpringCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpringCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//Velocity = FVector::ZeroVector;
	
	GetOwner()->GetComponents(SpringArray);
}


// Called every frame
void USpringCharacterMovementComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	FVector Location = GetOwner()->GetActorLocation();

	FString DebugMessage = "Position: " + Location.ToString() +
		"\nVelocity: " + Velocity.ToString();
	
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, DebugMessage);
	}

	Velocity += Gravity * DeltaTime;
	Velocity += NextFrameForceAdd;

	FHitResult* MoveResult = nullptr;
	GetOwner()->AddActorWorldOffset(Velocity * DeltaTime, true, MoveResult, ETeleportType::None);

	if (MoveResult && MoveResult->bBlockingHit) {
		Velocity = Velocity.MirrorByVector(MoveResult->Normal) * 0.05;
	}

	FVector SpringForceVector = FVector::ZeroVector;

	for (auto& Spring : SpringArray) {
		float SpringForce = Spring->GetSpringForce();
		FVector SpringDir = Spring->GetComponentRotation().Vector();

		SpringForceVector -= SpringDir * SpringForce;
		
		FString DebugText = "Force: " + FString::SanitizeFloat(SpringForce) + ", Direction" + SpringDir.ToString() + ", AddForce: " + SpringForceVector.ToString();
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, DebugText);
	}

	NextFrameForceAdd = SpringForceVector;

	
}

