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
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	// ...
}

float USpringCharacterLegSpring::GetLandingMult(FVector Velocity, float DeltaTime) {
	
	FVector SpringLocStart = GetComponentLocation();
	FVector SpringLocEnd = SpringLocStart + GetComponentRotation().Vector() * SpringLength;
	FVector SpringLocEndNextFrame = SpringLocEnd + Velocity * DeltaTime;

	
	FHitResult HitResult;
	FCollisionObjectQueryParams Params(ECollisionChannel::ECC_WorldStatic);

	float ReturnMult = 0.f;

	if (Grounded) {
		ReturnMult = 1.f;
		
		Grounded = GetWorld()->LineTraceSingleByObjectType(HitResult, SpringLocStart, SpringLocEndNextFrame, Params);

		if (!Grounded) {
			// No hit, do another line trace to get impact point
			bool AnyHit = GetWorld()->LineTraceSingleByObjectType(HitResult, SpringLocStart, SpringLocEnd, Params);

			if (AnyHit) {
				ReturnMult = (HitResult.ImpactPoint - SpringLocEnd).Size() / (SpringLocEnd - SpringLocEndNextFrame).Size();
			} else {
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "No hit...");
			}
		}

	// If not grounded last frame
	} else {
		Grounded = GetWorld()->LineTraceSingleByObjectType(HitResult, SpringLocStart, SpringLocEndNextFrame, Params);

		if (Grounded) {
			ReturnMult = (HitResult.ImpactPoint - SpringLocEndNextFrame).Size() / (SpringLocEnd - SpringLocEndNextFrame).Size();
		}
	}

	FString DebugText = "ReturnMult: " + FString::SanitizeFloat(ReturnMult);
	GEngine->AddOnScreenDebugMessage(15, 2, FColor::Blue, DebugText);

	return ReturnMult;
}

float USpringCharacterLegSpring::GetSpringForce(FVector Velocity, float DeltaTime) {
	
	float Force = GetLandingMult(Velocity, DeltaTime) * ForceThrustMax;
	
	return Force;
}
