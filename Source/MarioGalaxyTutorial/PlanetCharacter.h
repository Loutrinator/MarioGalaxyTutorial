// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlanetPawn.h"
#include "Components/CapsuleComponent.h"
#include "PlanetCharacter.generated.h"

class UTextBlock;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateScoreText, FString, _nbCatched);

UCLASS()
class MARIOGALAXYTUTORIAL_API APlanetCharacter : public APlanetPawn
{
	GENERATED_BODY()

private:
	UCapsuleComponent* capsule;
	FVector ForwardMovementDirection;
	FVector JumpDirection;
	float RotationValue;
	int nbCatched;
	
public:
	APlanetCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float MovementSpeed = 10.f;
	UPROPERTY(EditAnywhere)
	float LerpSpeed = 10.f;
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 10.f;
	UPROPERTY(EditAnywhere)
	float JumpForce = 10.f;
	UPROPERTY(EditAnywhere)
	float JumpLerpSpeed = 10.f;
	
	UPROPERTY(EditAnywhere)
	bool enableRotation = true;
	
	UPROPERTY(EditAnywhere)
	bool enableMovement = true;

	UPROPERTY(BlueprintAssignable)
	FUpdateScoreText delegateUpdateText;
	
	int GetCatchedCount();
	void CatchEscapingPawn();
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Jump();
	// End of APawn interface

};
