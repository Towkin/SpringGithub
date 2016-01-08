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
	
	TArray<USpringCharacterLegSpring*> SpringArray;
	//FVector Velocity = FVector::ZeroVector;
	FVector Gravity = FVector(0.f, 0.f, -982.f);
	float Mass = 100;
};
