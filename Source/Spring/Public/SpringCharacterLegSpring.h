// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "SpringCharacterLegSpring.generated.h"


UCLASS( ClassGroup=(Spring), meta=(BlueprintSpawnableComponent) )
class SPRING_API USpringCharacterLegSpring : public USceneComponent
{
	GENERATED_BODY()

private:
	// Check if you are landing the next frame, prepare time-precise force multiplier
	float GetLandingMult();
	
	virtual void USpringCharacterLegSpring::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

public:	
	// Sets default values for this component's properties
	USpringCharacterLegSpring();

	UPROPERTY()
	class UArrowComponent* ArrowComponent;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	FHitResult LastHit;
	bool Grounded = false;
	FVector GroundedLocation = FVector::ZeroVector;
	bool Accelerating = false;
	float PotentialVelocity = 0.f;

	FVector GroundVector = FVector(0.f, 0.f, -1.f);
	float GroundScalar = 1.f;
	FVector OffsetVector = FVector(0.f);

	float RotationLerpSpeed = 0.25f;

	
	void VelocityModify(FVector& Velocity, float DeltaTime);
	float GetSpringForce();

	UPROPERTY(Category = "Spring", EditAnywhere, BlueprintReadWrite)
	float ForceThrustMax = 1250000.0f;

	UPROPERTY(Category = "Spring", EditAnywhere, BlueprintReadWrite)
	float SpringLength = 60.f;
};
