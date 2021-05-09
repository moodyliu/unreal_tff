// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerTFF.h"
#include "CameraShakeTFF.h"

void APlayerControllerTFF::MakeCameraShake()
{
	PlayerCameraManager->StartCameraShake(UCameraShakeTFF::StaticClass());

	// PlayerCameraManager->CameraStyle = FName(TEXT("FirstPerson"));
}

void APlayerControllerTFF::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("Shake"),EInputEvent::IE_Pressed,this,&APlayerControllerTFF::MakeCameraShake);
}
