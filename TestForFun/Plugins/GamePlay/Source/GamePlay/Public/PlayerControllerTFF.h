// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerTFF.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAY_API APlayerControllerTFF : public APlayerController
{
	GENERATED_BODY()
	
	void MakeCameraShake();

	void SetupInputComponent() override;
};
