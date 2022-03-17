// Copyright Epic Games, Inc. All Rights Reserved.

#include "MarioGalaxyTutorialCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AMarioGalaxyTutorialCharacter

AMarioGalaxyTutorialCharacter::AMarioGalaxyTutorialCharacter()
{
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PrimaryActorTick.bCanEverTick = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GravityScale = 0.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


}


void AMarioGalaxyTutorialCharacter::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	if (RootComponent == nullptr) GEngine->AddOnScreenDebugMessage(0, 2.5, FColor::Red, "RootComponent nullptr");

	FVector characterForward = RootComponent->GetForwardVector();

	FVector pos = GetActorLocation();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, pos.ToString());
	FVector up = pos;
	up.Normalize();
	DrawDebugLine(GetWorld(), pos, pos + up*100, FColor::Green, false, deltaTime, ECC_WorldStatic, 1.f);


	FVector right = FVector::CrossProduct(up, characterForward);
	FVector forward = FVector::CrossProduct(right, up);
	FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(forward, up);
	this->SetActorRotation(Rot);
	
	USkeletalMeshComponent* skeletal = this->FindComponentByClass<USkeletalMeshComponent>();
	if(skeletal == nullptr) GEngine->AddOnScreenDebugMessage(0, 2.5, FColor::Red, "USkeletalMeshComponent nullptr");
	
	//FTransform newTransform(forward, right, up, pos);

	FVector gravity = -up * 9.81f * 100.f;
	DrawDebugLine(GetWorld(), pos, pos + gravity*100, FColor::Yellow, false, deltaTime, ECC_WorldStatic, 3.f);
	skeletal->SetPhysicsLinearVelocity(gravity,true);
	GetCharacterMovement()->AddForce(gravity);
	/*
	
	*/
	//this->GetMovementComponent()->Velocity += gravity;
	//this->LaunchCharacter(,false,false);
}


//////////////////////////////////////////////////////////////////////////
// Input

void AMarioGalaxyTutorialCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMarioGalaxyTutorialCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMarioGalaxyTutorialCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMarioGalaxyTutorialCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMarioGalaxyTutorialCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMarioGalaxyTutorialCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMarioGalaxyTutorialCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMarioGalaxyTutorialCharacter::OnResetVR);
}


void AMarioGalaxyTutorialCharacter::OnResetVR()
{
	// If MarioGalaxyTutorial is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in MarioGalaxyTutorial.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMarioGalaxyTutorialCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMarioGalaxyTutorialCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMarioGalaxyTutorialCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMarioGalaxyTutorialCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMarioGalaxyTutorialCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMarioGalaxyTutorialCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
