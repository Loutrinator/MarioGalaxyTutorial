// Fill out your copyright notice in the Description page of Project Settings.


#include "PlanetPawn.h"

#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlanetPawn::APlanetPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Game/Meshes/PlanetCube.PlanetCube"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	StaticMeshComponent->SetStaticMesh(CubeMesh.Object);*/

}

// Called when the game starts or when spawned
void APlanetPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlanetPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (RootComponent == nullptr) GEngine->AddOnScreenDebugMessage(0, 2.5, FColor::Red, "RootComponent nullptr");

	FVector characterForward = RootComponent->GetForwardVector();

	FVector pos = GetActorLocation();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, pos.ToString());
	FVector up = pos;
	up.Normalize();
	DrawDebugLine(GetWorld(), pos, pos + up*100, FColor::Green, false, DeltaTime, ECC_WorldStatic, 1.f);


	FVector right = FVector::CrossProduct(up, characterForward);
	FVector forward = FVector::CrossProduct(right, up);
	FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(forward, up);
	this->SetActorRotation(Rot);
	
	UStaticMeshComponent* mesh = this->FindComponentByClass<UStaticMeshComponent>();
	if(mesh == nullptr) GEngine->AddOnScreenDebugMessage(0, 2.5, FColor::Red, "USkeletalMeshComponent nullptr");
	
	//FTransform newTransform(forward, right, up, pos);

	FVector gravity = -up * 9.81f * GravityScale;
	DrawDebugLine(GetWorld(), pos, pos + gravity*100, FColor::Yellow, false, DeltaTime, ECC_WorldStatic, 3.f);
	mesh->SetPhysicsLinearVelocity(gravity,true);
	//GetCharacterMovement()->AddForce(gravity);
}

// Called to bind functionality to input
void APlanetPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

