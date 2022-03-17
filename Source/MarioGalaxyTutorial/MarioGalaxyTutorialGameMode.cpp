// Copyright Epic Games, Inc. All Rights Reserved.

#include "MarioGalaxyTutorialGameMode.h"
#include "MarioGalaxyTutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMarioGalaxyTutorialGameMode::AMarioGalaxyTutorialGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/MyPlanetCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
