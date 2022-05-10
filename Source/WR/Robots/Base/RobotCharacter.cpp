// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotCharacter.h"
#include "Engine/Engine.h"
#include <Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>

// Sets default values
ARobotCharacter::ARobotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    // SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

    // Called when the game starts or when spawned
    void ARobotCharacter::BeginPlay()
    {
	    Super::BeginPlay();
	
    }

    // Called every frame
    void ARobotCharacter::Tick(float deltaTime)
    {
	    Super::Tick(DeltaTime);
        DeltaTime = deltaTime;
    }

    // Called to bind functionality to input
    void ARobotCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
    {
	    Super::SetupPlayerInputComponent(PlayerInputComponent);
        PlayerInputComponent->BindAxis("TurnAround", this, &ARobotCharacter::TurnAround);
        PlayerInputComponent->BindAxis("MoveForward", this, &ARobotCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &ARobotCharacter::MoveRight);
    }
    
    void ARobotCharacter::TurnAround(float amount)
    {
        FRotator contrRot = this->GetControlRotation();
        amount = FMath::Lerp<float, float>(PreviousTurnAmount, amount, RotationDamping * (1 - FMath::Exp(-20 * DeltaTime)));
       
        AddControllerYawInput(amount);
        PreviousTurnAmount = amount;
        HipsTurn -= amount;
       // HipsTurn = UKismetMathLibrary::NormalizeAxis(FMath::Fmod(HipsTurn, 360));
        //HipsTurn = FMath::Fmod(HipsTurn, 360);
        FString Stat = FString::Printf(TEXT("HipsTurn: %f"), HipsTurn);
        GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Yellow, Stat);
        FString Stat2 = FString::Printf(TEXT("turn target: %f"), TurnTarget);
        GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Red, Stat2);
    }

    void ARobotCharacter::MoveForward(float amount)
    {
        if (amount == 0)
        {
            return;
        }
        TurnTarget = HipsTurn - FMath::Fmod(HipsTurn, 360); 
          FString Stat2 = FString::Printf(TEXT("target int: %f"), TurnTarget);
        GEngine->AddOnScreenDebugMessage(4, 2.0f, FColor::Green, Stat2);
        if (amount > 0)
        {
            //HipsTurn = UKismetMathLibrary::FInterpEaseInOut(HipsTurn, 0, 0.2, 3);
            TurnTarget = TurnTarget + ((HipsTurn - TurnTarget) > 180 ? 360 : (HipsTurn - TurnTarget) < -180 ? -360 : 0);
            HipsTurn = FMath::Lerp<float, float>(HipsTurn, TurnTarget, 0.2 * (1 - FMath::Exp(-20 * DeltaTime)));
            AddMovementInput(GetActorForwardVector(), amount * 3);
        }
        else if (amount < 0)
        {
            TurnTarget += 180;
            TurnTarget = TurnTarget + (HipsTurn - TurnTarget > 180 ? 360 : 0);
            HipsTurn = FMath::Lerp<float, float>(HipsTurn, TurnTarget, 0.2 * (1 - FMath::Exp(-20 * DeltaTime)));
            AddMovementInput(GetActorForwardVector(), amount * 3);
        }
    }

    void ARobotCharacter::MoveRight(float amount)
    {
        if (amount == 0)
        {
            return;
        }
        float mod = FMath::Fmod(HipsTurn, 360);
        TurnTarget = HipsTurn - mod; 
        if (amount < 0)
        {
            TurnTarget += 270;
            TurnTarget = TurnTarget + ((HipsTurn - TurnTarget) > 180 ? 360 : (HipsTurn - TurnTarget) < -180 ? -360 : 0);
            HipsTurn = FMath::Lerp<float, float>(HipsTurn, TurnTarget,
                    0.2 * (1 - FMath::Exp(-20 * DeltaTime)));
            AddMovementInput(GetActorRightVector(), amount * 3);
        } else if (amount > 0)
        {
            TurnTarget += 90;
            HipsTurn =
                FMath::Lerp<float, float>(HipsTurn, TurnTarget + ((HipsTurn - TurnTarget) > 180 ? 360 : 0), 0.2 * (1 - FMath::Exp(-20 * DeltaTime)));
            AddMovementInput(GetActorRightVector(), amount * 3);
        }
    }


