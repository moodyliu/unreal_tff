// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestForFunGameMode.h"
#include "TestForFunCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestForFunGameMode::ATestForFunGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
