// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SpringPawnMovementComponent.h"

#include "SpringPawn.generated.h"


/**
 * Spring Pawn, with the basic components needed for all Spring Characters.
 * Includes Camera, Capsule component and a SpringPawnMovementComponent.
 */

UCLASS()
class SPRING_API ASpringPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = SpringPawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* RootCapsule;
	
	UPROPERTY(Category = SpringPawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;
	
	UPROPERTY(Category = SpringPawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringPawnMovementComponent* MoveComponent;
	
	virtual USpringPawnMovementComponent* GetMovementComponent() const override;
	
	
	// Sets default values for this pawn's properties
	ASpringPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
};
