// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakeTFF.h"

UCameraShakeTFF::UCameraShakeTFF(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AnimPlayRate = 1.f;
	AnimScale = 1.0f;
	AnimBlendInTime = 0.2f;
	AnimBlendOutTime = 0.2f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.2f;
	OscillationDuration = .5f;
	RotOscillation.Pitch.Amplitude = 5.f;
	RotOscillation.Pitch.Frequency = 50.f;
	RotOscillation.Yaw.Amplitude = 5.f;
	RotOscillation.Yaw.Frequency = 50.f;
	RotOscillation.Roll.Amplitude = 5.f;
	RotOscillation.Roll.Frequency = 50.f;
}