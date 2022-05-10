// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "Engine/Engine.h"
#include <Runtime/Engine/Classes/Kismet/KismetTextLibrary.h>
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    //SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
    YawTurn = 0;
    HipsTurn = 0;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    CalculateTurn(AmountTurn);
    CalculateForward(AmountForward);
    CalculateRight(AmountRight);
    HipsTurnPublic = HipsTurn;
    if (HipsTurnPublic != 0.0f)
    {
        GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Red, FString::SanitizeFloat(HipsTurnPublic));
    }
   
}

    // Called to bind functionality to input
    void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
    {
	    Super::SetupPlayerInputComponent(PlayerInputComponent);
        PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
        PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
        PlayerInputComponent->BindAxis("TurnAround", this, &AMainCharacter::TurnAround);

    }

    void AMainCharacter::MoveForward(float amount)
    {
       
        AmountForward = amount * 2.5;
        AddMovementInput(GetActorForwardVector(), amount);
    }

    void AMainCharacter::MoveRight(float amount)
    {
        AmountRight = amount * 1.5;
       // AmountRight = FMath::Lerp<float, int>( amount * 1.5;
        AddMovementInput(GetActorRightVector(), amount * 3);
       
    }

    void AMainCharacter::TurnAround(float amount)
    {
        FText amountT = UKismetTextLibrary::Conv_FloatToText(amount, ERoundingMode::HalfToEven, false, true, 1, 324, 0, 1);
        FString Stat = FString::Printf(TEXT("Turn amount: %s"), *amountT.ToString());
        GEngine->AddOnScreenDebugMessage(6, 2.0f, FColor::Yellow, Stat);
        FString amountS = FString::SanitizeFloat(amount, 2);  // amount < 0 ? -3 : 3;
        AmountTurn = FCString::Atof(*amountT.ToString());
        GEngine->AddOnScreenDebugMessage(4, 2.0f, FColor::Green, FString::SanitizeFloat(AmountTurn));
    }

    void AMainCharacter::CalculateMove(float amountForward, float amountRight) {
        if (amountForward == 0 && amountRight == 0)
        {
            return;
        }
    }

    void AMainCharacter::CalculateRight(float amount) {
        GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, FString::SanitizeFloat(amount));
        IsMoveRightPreviousFrame = IsMoveRight;
        IsMoveRight = this->GetVelocity().Size() != 0 && (90 - FMath::Abs(HipsTurn) < 40);
        if (amount == 0)
        {
            return;
        }
        if (HipsTurn + amount <= -90)
        {
            HipsTurn = -90;
        }
        if (HipsTurn + amount >= 90)
        {
            HipsTurn = 90;
        }

        if (HipsTurn + amount > -90 && HipsTurn + amount < 90)
        {
            HipsTurn += amount;
        }
        if (FMath::IsNearlyEqual(HipsTurn, -90.0f) || FMath::IsNearlyEqual(HipsTurn, 90.0f) || IsMoveRightPreviousFrame ||
            IsMoveForwardPreviousFrame)
        {
           // GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Blue, TEXT("Can move right"));
            AddMovementInput(GetActorRightVector(), amount * 3);
        }
    }

    void AMainCharacter::CalculateForward(float amount)
    {
      
        IsMoveForwardPreviousFrame = IsMoveForward;
        IsMoveForward = this->GetVelocity().Size() != 0 && (FMath::Abs(HipsTurn) < 40 || 180 - FMath::Abs(HipsTurn) < 40);
      //  GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Blue, FString::SanitizeFloat(HipsTurn));
        // GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Green, FString::SanitizeFloat(this->rotatio().Yaw));
        if (amount == 0)
        {
            return;
        }
  
        if (HipsTurn == 0 && amount < 0)
        {
           // HipsTurn += amount;
            return;
        }
        if (FMath::IsNearlyEqual(HipsTurn, 180.0f) && amount > 0)
        {
            HipsTurn -= amount;
            return;
        }
        if (FMath::IsNearlyEqual(HipsTurn, -180.0f) && amount > 0)
        {
            HipsTurn += amount;
            return;
        }
    
        if (HipsTurn == 0 || IsMoveForwardPreviousFrame || IsMoveRightPreviousFrame || FMath::IsNearlyEqual(HipsTurn, 180.0f) ||
            FMath::IsNearlyEqual(HipsTurn, -180.0f))
        {
           // GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Blue, TEXT("Can move forward"));
            AddMovementInput(GetActorForwardVector(), amount*3);
        }
        if ((HipsTurn <= 0 && HipsTurn + amount >= 0) || (HipsTurn >= 0 && HipsTurn + amount <= 0))
        {
           // HipsTurn = 0;
        }
        else if (HipsTurn > -180 && HipsTurn < 0 && amount > 0)
        {
            //---> 0
           // HipsTurn += amount;
        }
        else if (HipsTurn > 0 && HipsTurn < 180 && amount > 0)
        {
            //---> 0
            amount = HipsTurn - amount < 0 ? HipsTurn : amount;
 
            //HipsTurn = FMath::Lerp<float, float>(HipsTurn, HipsTurn - amount, 0.9);
            FString Stat = FString::Printf(TEXT("forward amount: %f"), amount);
            GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, Stat);
            HipsTurn -= amount;
            GEngine->AddOnScreenDebugMessage(7, 2.0f, FColor::Cyan, FString::Printf(TEXT("HipsTurn: %f"), HipsTurn));
           
        }
        else if (HipsTurn > -180 && HipsTurn < 0 && amount < 0)
        {
            //---> -180
            if (HipsTurn + amount <= -180)
            {
                HipsTurn = -180.0f;
            }
            else
            {
                HipsTurn += amount;
            }
        }
        else if (HipsTurn > 0 && HipsTurn < 180 && amount < 0)
        {
            //---> 180
            if (HipsTurn - amount >= 180)
            {
                HipsTurn = 180.0f;
            }
            else
            {
                HipsTurn -= amount;
            }
        }
       
    }

    void AMainCharacter::CalculateTurn(float amount) {

    if (amount == 0)
    {
        return;
    }
  
    //YawTurn = FMath::Lerp<float, float>( YawTurn, YawTurn+amount, 0.9);
    YawTurn += amount;
     HipsTurn -= amount;
  
    if (HipsTurn < -180.0f)
    {
        HipsTurn = 180.0f - (FMath::Abs(HipsTurn) - 180.0f);
    }
    if (HipsTurn > 180.0f)
    {
        HipsTurn = -180.0f + (HipsTurn - 180.0f);
       
    }
    // AddControllerYawInput(amount);
  
    FRotator doorRot = SpringArmComponent->GetRelativeRotation();
    AMainPlayerController* controller = this->GetController<AMainPlayerController>();
    FRotator rot = controller->GetControlRotation();
    FRotator newRot = FRotator(rot.Euler().Y, YawTurn, rot.Euler().X);
  
  //  HipsTurn = -rot.Euler().Z;
   controller->SetControlRotation(newRot);
    UCharacterMovementComponent* mc = this->GetCharacterMovement();
 
      FRotator actorRot = this->GetActorRotation();
    FRotator newActorRot = FRotator(actorRot.Euler().Y, YawTurn, actorRot.Euler().X);
   //   this->SetActorRotation(newActorRot);

   
  
    FRotator DoorNewRotation = FRotator(doorRot.Euler().Y, newRot.Euler().Z, doorRot.Euler().X);
   // SpringArmComponent->SetRelativeRotation(DoorNewRotation);
}

