// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanetPawn.h"
#include "EscapingPlanetPawn.generated.h"

/**
 * 
 */
UCLASS()
class MARIOGALAXYTUTORIAL_API AEscapingPlanetPawn : public APlanetPawn
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere)
	float speed = 5.f;
	
	UPROPERTY(EditAnywhere)
	float playerEscapeDistance = 400.0f;
	virtual void Tick(float deltaTime) override;
};
