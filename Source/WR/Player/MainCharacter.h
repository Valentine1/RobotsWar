// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class WR_API AMainCharacter : public ACharacter
{
    GENERATED_BODY()

private:
    bool IsMoveForward;
    bool IsMoveForwardPreviousFrame;
    bool IsMoveRight;
    bool IsMoveRightPreviousFrame;
    float AmountForward;
    float AmountRight;
    float AmountTurn;

    void CalculateForward(float amount);
    void CalculateRight(float amount);
    void CalculateTurn(float amount);
    void CalculateMove(float amountForward, float amountRight);

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float YawTurn;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float HipsTurn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float HipsTurnPublic;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    void MoveForward(float Amount);
    void MoveRight(float Amount);
   

    void TurnAround(float Amount);
};
