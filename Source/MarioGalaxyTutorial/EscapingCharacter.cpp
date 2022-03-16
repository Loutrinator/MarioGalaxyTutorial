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
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
		UE_LOG(LogTemp, Log, TEXT("Object name : %s"), Iterator->Get()->GetFName());
		
    }
}

// Called to bind functionality to input
void AEscapingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

