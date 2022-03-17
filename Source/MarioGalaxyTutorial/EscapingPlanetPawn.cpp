// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapingPlanetPawn.h"

void AEscapingPlanetPawn::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	float speed = 1;

	float playerDist = GetDistanceTo(GetWorld()->GetFirstPlayerController());
	if(playerDist > 10) return;

	auto playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	auto direction = GetActorLocation() - playerLocation;
	direction.Z = 0;
	direction.Normalize();

	SetActorLocation(GetActorLocation() + direction * speed);
}
