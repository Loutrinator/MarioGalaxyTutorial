// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "TriggerCollectEscapingPawn.generated.h"

/**
 * 
 */
UCLASS()
class MARIOGALAXYTUTORIAL_API ATriggerCollectEscapingPawn : public ATriggerBox
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnOverlapBegin(AActor* Actor, AActor* other);
	ATriggerCollectEscapingPawn();
};
