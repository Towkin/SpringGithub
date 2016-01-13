// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "SpringCharacterLegSpring.h"
#include "SpringPawnMovementComponent.generated.h"

UCLASS()
class SPRING_API USpringPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
private:
	FVector NextFrameVelocityAdd = FVector::ZeroVector;
public:
	USpringPawnMovementComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void AddInputVector(FVector WorldVector, bool bForce = false) override;
	
	FVector CalculateLocationAtTime(FVector StartLocation, FVector StartingVelocity, float Time, FVector ConstantForce);

	// MovementBounds

	UPROPERTY(Category = "Physics", EditAnywhere, BlueprintReadWrite)
	int32 FixedFrameRate = 120;
	UPROPERTY(Category = "Physics", EditAnywhere, BlueprintReadWrite)
	int32 MaxUpdatesPerFrame = 10;
	float FixedDeltaTime = 0.01f;
	float Accumulator = 0.f;
	virtual void FixedTick(float DeltaTime);

	TArray<USpringCharacterLegSpring*> SpringArray;
	FVector Gravity = FVector(0.f, 0.f, -982.f);
	float Mass = 100;

	float FactorDefaultZ = 2.0f;
	float FactorSprintZ = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "MovementController|Input", meta = (Keywords = "Sprint"))
	virtual void SetSprint(bool SprintEnabled);

	float DirectionFactorZ = FactorDefaultZ;
	FVector SpringDirectionVector = FVector(0.f);

	bool MovementGrounded = false;

	UPROPERTY(Category = "AirMove", EditAnywhere, BlueprintReadWrite)
	float AirMoveForce = 35000.f;

	

};
