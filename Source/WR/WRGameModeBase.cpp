// Copyright Epic Games, Inc. All Rights Reserved.


#include "WRGameModeBase.h"
#include "Player/MainCharacter.h"
#include "Player/MainPlayerController.h"

AWRGameModeBase::AWRGameModeBase() {
    DefaultPawnClass = AMainCharacter::StaticClass();
    PlayerControllerClass = AMainPlayerController::StaticClass();
}