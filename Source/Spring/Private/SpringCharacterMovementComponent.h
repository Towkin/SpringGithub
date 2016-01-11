// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SpringCharacterLegSpring.h"
#include "SpringCharacterMovementComponent.generated.h"


UCLASS( ClassGroup=(Spring), meta=(BlueprintSpawnableComponent) )
class USpringCharacterMovementComponent : public UActorComponent {
	GENERATED_BODY()

private:

	FVector Velocity = FVector::ZeroVector;
	FVector Gravity = FVector(0.f, 0.f, -9.82f);
	FVector NextFrameForceAdd = FVector::ZeroVector;

public:
	// Sets default values for this component's properties
	USpringCharacterMovementComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UPROPERTY(Category="Spring", BlueprintReadOnly)
	TArray<USpringCharacterLegSpring*> SpringArray;


};
