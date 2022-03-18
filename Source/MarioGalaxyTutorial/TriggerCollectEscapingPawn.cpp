// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerCollectEscapingPawn.h"

#include "PlanetCharacter.h"

void ATriggerCollectEscapingPawn::OnOverlapBegin(AActor* Actor, AActor* other)
{
	if(other && other != this)
	{
		APlanetCharacter* planetCharacter = Cast<APlanetCharacter>(other);
		// todo planetCharacter collects this item
		planetCharacter->CatchEscapingPawn();
		Actor->GetParentActor()->Destroy();
	}
}

ATriggerCollectEscapingPawn::ATriggerCollectEscapingPawn()
{
	OnActorBeginOverlap.AddDynamic(this, &ATriggerCollectEscapingPawn::OnOverlapBegin);
}
