// Fill out your copyright notice in the Description page of Project Settings.

#include "Spring.h"
#include "SpringCharacterLegSpring.h"


// Sets default values for this component's properties
USpringCharacterLegSpring::USpringCharacterLegSpring()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpringCharacterLegSpring::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpringCharacterLegSpring::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...

	
}

float USpringCharacterLegSpring::GetLandingMult(FVector Velocity, float DeltaTime) {
	
	FVector SpringLocStart = GetComponentLocation();
	FVector SpringLocEnd = SpringLocStart + GetComponentRotation().Vector() * SpringLength;
	FVector SpringLocEndNextFrame = SpringLocEnd + Velocity * DeltaTime;

	
	FHitResult HitResult;
	FCollisionObjectQueryParams Params(ECollisionChannel::ECC_WorldStatic);
	
	

	Grounded = GetWorld()->LineTraceSingleByObjectType(HitResult, SpringLocStart, SpringLocEndNextFrame, Params);
	
	float ReturnMult = 0.f;

	if (Grounded) {
		ReturnMult = HitResult.PenetrationDepth / (SpringLocEnd - SpringLocEndNextFrame).Size();
	}

	FString DebugText = "ReturnMult: " + FString::SanitizeFloat(ReturnMult);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, DebugText);

	return ReturnMult;
}

float USpringCharacterLegSpring::GetSpringForce(FVector Velocity, float DeltaTime) {
	float Force = GetLandingMult(Velocity, DeltaTime) * ForceThrustMax;
	

	return Force;
}
