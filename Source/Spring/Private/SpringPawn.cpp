// Fill out your copyright notice in the Description page of Project Settings.

#include "Spring.h"
#include "SpringPawn.h"


// Sets default values
ASpringPawn::ASpringPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	

	RootCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootCapsule"));
	RootCapsule->InitCapsuleSize(34.0f, 50.0f);

	RootComponent = RootCapsule;

	static FName CollisionProfileName(TEXT("Pawn"));
	RootCapsule->SetCollisionProfileName(CollisionProfileName);

	RootCapsule->CanCharacterStepUpOn = ECB_No;
	RootCapsule->bShouldUpdatePhysicsVolume = true;
	RootCapsule->bCheckAsyncSceneOnMove = false;
	RootCapsule->bCanEverAffectNavigation = false;
	RootCapsule->bDynamicObstacle = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->AttachTo(RootCapsule);
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	PlayerCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	MoveComponent = CreateDefaultSubobject<USpringPawnMovementComponent>(TEXT("MovementComponent"));
	MoveComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void ASpringPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpringPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASpringPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

USpringPawnMovementComponent* ASpringPawn::GetMovementComponent() const {
	return MoveComponent;
}