// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapingCharacter.h"

// Sets default values
AEscapingCharacter::AEscapingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEscapingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEscapingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float speed = 1;

	float playerDist = GetDistanceTo(GetWorld()->GetFirstPlayerController());
	if(playerDist > 10) return;
	
	auto playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	auto direction = GetActorLocation() - playerLocation;
	direction.Z = 0;
	direction.Normalize();

	SetActorLocation(GetActorLocation() + direction * speed);
}

// Called to bind functionality to input
void AEscapingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

