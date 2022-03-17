// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapingPlanetPawn.h"

#include <string>

#include "EngineUtils.h"
#include "PlanetCharacter.h"
#include "Kismet/GameplayStatics.h"

void AEscapingPlanetPawn::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	APlanetCharacter* planetCharacter = nullptr;
	for (TActorIterator<APlanetCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		planetCharacter = Cast<APlanetCharacter>(*ActorItr);
		if (planetCharacter)
		{
			break;
		}
	}
	if(planetCharacter == nullptr) return;
	float playerDist = GetDistanceTo(planetCharacter);
	if(playerDist > playerEscapeDistance) return;

	auto playerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	auto direction = GetActorLocation() - playerLocation;
	direction.Z = 0;
	direction.Normalize();

	SetActorLocation(GetActorLocation() + direction * speed);
}
