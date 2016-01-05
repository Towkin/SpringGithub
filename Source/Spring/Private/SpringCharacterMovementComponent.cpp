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

	//Velocity += Gravity * DeltaTime;

	FVector AddForce = FVector::ZeroVector;

	for (auto& Spring : SpringArray) {
		float SpringForce = Spring->GetSpringForce(Velocity, DeltaTime);
		FVector SpringDir = Spring->GetComponentRotation().Vector();

		AddForce -= SpringDir * SpringForce;
		
		FString DebugText = "Force: " + FString::SanitizeFloat(SpringForce) + ", Direction" + SpringDir.ToString() + ", AddForce: " + AddForce.ToString();
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, DebugText);
	}

	//Velocity += AddForce * DeltaTime;

	//FHitResult* SweepResult = nullptr;

	//GetOwner()->AddActorWorldOffset(Velocity, true, SweepResult);

	//if (SweepResult != nullptr) {
	//	if (GEngine) {
	//		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Blue, "Hit Ground!");
	//	}
		//Velocity = Velocity.MirrorByVector(SweepResult->ImpactNormal);
	//}
	
	
}

