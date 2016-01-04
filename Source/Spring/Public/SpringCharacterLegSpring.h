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
	float GetLandingMult(FVector& Velocity, float DeltaTime);
	
	
	float PotentialEnergy = 0;
	bool Grounded = false;

public:	
	// Sets default values for this component's properties
	USpringCharacterLegSpring();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//UPROPERTY(VisibleAnywhere, Category="SpringComponents")
	//class UArrowComponent* Arrow;

	
	float GetSpringForce(FVector& Velocity, float DeltaTime);

	float ForceThrustMax = 1250.0f;
	float SpringLength = 60.f;

};
