// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class SPRING_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// TODO: Actually do something :P
	void MovePlatform(FVector TargetLocation, float OverTime);
	
	//FVector CurrentTargetLocation = FVector::ZeroVector;

	/**
	 * The actor's velocity changes how fast or slow the object is moving.
	 * It's used in it's Tick-function.
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Platform")
	FVector Velocity = FVector::ZeroVector;


};
