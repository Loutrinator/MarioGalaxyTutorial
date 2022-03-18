// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetCharacter.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

APlanetCharacter::APlanetCharacter()
{
	capsule = FindComponentByClass<UCapsuleComponent>();
	nbCatched = 0;
}

// Called every frame
void APlanetCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector zero(0, 0, 0);
	ForwardMovementDirection = FMath::Lerp(ForwardMovementDirection, zero, DeltaTime * LerpSpeed);
	JumpDirection = FMath::Lerp(JumpDirection, zero, DeltaTime * JumpLerpSpeed);
	RotationValue = FMath::Lerp(RotationValue, 0.f, DeltaTime * LerpSpeed);

	if (enableRotation)
	{
		FQuat quaternion(GetActorUpVector(), RotationValue * DeltaTime * RotationSpeed);
		quaternion = FMath::Lerp(quaternion * GetActorQuat(), GetActorQuat(), DeltaTime * LerpSpeed);
		SetActorRotation(quaternion);
	}

	if (enableMovement)
	{
		const FVector NewLocation = GetActorLocation() + (ForwardMovementDirection * DeltaTime * MovementSpeed) + JumpDirection * JumpForce * DeltaTime;
		
		SetActorLocation(NewLocation);
	}
	//capsule->SetPhysicsAngularVelocity(FVector());
	//SetActorRotation();
}

void APlanetCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green, "SetupPlayerInputComponent");
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlanetCharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlanetCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlanetCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AMarioGalaxyTutorialCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AMarioGalaxyTutorialCharacter::LookUpAtRate);

	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &AMarioGalaxyTutorialCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &AMarioGalaxyTutorialCharacter::TouchStopped);

	// VR headset functionality
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMarioGalaxyTutorialCharacter::OnResetVR);
}


void APlanetCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		GEngine->AddOnScreenDebugMessage(0, 0.2f, FColor::Green, "MoveForward");

		// get forward vector
		const FVector Direction = GetActorForwardVector();

		ForwardMovementDirection = Direction * Value;
	}
}

void APlanetCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement in that direction
		RotationValue = Value;
	}
}

void APlanetCharacter::Jump()
{
	FVector Direction = GetActorForwardVector();
	Direction += GetActorUpVector()*0.7f;
	Direction.Normalize();
	JumpDirection = Direction;
}

int APlanetCharacter::GetCatchedCount()
{
	return nbCatched;
}

void APlanetCharacter::CatchEscapingPawn()
{
	++nbCatched;
	delegateUpdateText.Broadcast(FString::FromInt(nbCatched));
}