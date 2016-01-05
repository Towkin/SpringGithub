// Fill out your copyright notice in the Description page of Project Settings.

#include "Spring.h"
#include "SpringCharacter.h"


// Sets default values
ASpringCharacter::ASpringCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpringCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Load all LegSprings into the SpringArray
	GetComponents(SpringArray);
}

// Called every frame
void ASpringCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector SpringForceVector = FVector::ZeroVector;

	for (auto& Spring : SpringArray) {
		float SpringForce = Spring->GetSpringForce(GetVelocity(), DeltaTime);
		FVector SpringDir = Spring->GetComponentRotation().Vector();

		SpringForceVector -= SpringDir * SpringForce;

		FString DebugText = "Force: " + FString::SanitizeFloat(SpringForce) + ", Direction" + SpringDir.ToString() + ", Force: " + SpringForceVector.ToString();
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, DebugText);
	}

	UPrimitiveComponent* ForceApplyObject = GetMovementBase();
	if (ForceApplyObject != nullptr) {
		ForceApplyObject->AddForce(SpringForceVector);
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::Red, "MovementBase is Null Reference");
	}
	
	

}

// Called to bind functionality to input
void ASpringCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

