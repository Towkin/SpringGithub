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

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("LegArrow");
	ArrowComponent->SnapTo(this);
	
	ArrowComponent->SetWorldScale3D(FVector(SpringLength / 100, 1.f, 1.f));
	// ...
}


// Called when the game starts
void USpringCharacterLegSpring::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

#if WITH_EDITOR
void USpringCharacterLegSpring::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
	
	ArrowComponent->SetWorldScale3D(FVector(SpringLength / 100, 1.f, 1.f));

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(901, 0.5, FColor::Blue, "Modified ArrowComponent's scale");
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif  

// Called every frame
void USpringCharacterLegSpring::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	
	
	FRotator TargetRotation = (GroundVector * GroundScalar + OffsetVector).Rotation();

	SetWorldRotation(FQuat::Slerp(GetComponentQuat(), TargetRotation.Quaternion(), RotationLerpSpeed).Rotator());
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(32, 2.0, FColor::Blue, "RotationVector: " + GetComponentRotation().Vector().ToString() + "TargetRotation: " + TargetRotation.Vector().ToString());
	}


	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	// ...
}

float USpringCharacterLegSpring::GetLandingMult() {
	
	FVector SpringLocStart = GetComponentLocation();
	FVector SpringLocEnd = SpringLocStart + GetComponentRotation().Vector() * SpringLength;

	//FVector SpringLocStartNextFrame = SpringLocStart + Velocity * DeltaTime;
	//FVector SpringLocEndNextFrame = SpringLocEnd + Velocity * DeltaTime;

	FHitResult HitResult;
	FCollisionObjectQueryParams Params(ECollisionChannel::ECC_WorldStatic);

	

	Grounded = GetWorld()->LineTraceSingleByObjectType(HitResult, SpringLocStart, SpringLocEnd, Params);

	float ReturnMult = 1.f - HitResult.Time;


	

	return ReturnMult;
}

float USpringCharacterLegSpring::GetSpringForce() {
	
	float Force = GetLandingMult() * ForceThrustMax;

	FString DebugText = "Force: " + FString::SanitizeFloat(Force) + " / " + FString::SanitizeFloat(ForceThrustMax);
	GEngine->AddOnScreenDebugMessage(4213, 2, FColor::Blue, DebugText);

	return Force;
}


void USpringCharacterLegSpring::VelocityModify(FVector& Velocity, float DeltaTime) {
	FVector SpringLocStart = GetComponentLocation();
	FVector SpringLocEnd = SpringLocStart + GetComponentRotation().Vector() * SpringLength;
	//FVector SpringLocEndNextFrame = SpringLocEnd + Velocity * DeltaTime;

	FHitResult HitResult;
	FCollisionObjectQueryParams Params(ECollisionChannel::ECC_WorldDynamic);
	
	Grounded = GetWorld()->LineTraceSingleByObjectType(HitResult, SpringLocStart, SpringLocEnd, Params);


}
