// Fill out your copyright notice in the Description page of Project Settings.


#include "VerletComponent.h"

UVerletComponent::UVerletComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	// bTickInEditor = true;
	bAutoActivate = true;

	ShowPoints.Reset();
	TestParticles.Reset();

	forcedir = FVector(0, 0, -1);

}

void UVerletComponent::OnRegister()
{
	Super::OnRegister();

	int32 XSideNum = 10;
	int32 YSideNum = 10;
	int32 TestParticleNum = XSideNum * YSideNum;
	int32 ConstrainNum = (4 * TestParticleNum - 2 * XSideNum - 2 * YSideNum) * 0.5;

	ShowPoints.Reset();
	TestParticles.Reset();
	Constrains.Reset();
	ShowPoints.AddUninitialized(TestParticleNum);
	TestParticles.AddUninitialized(TestParticleNum);
	Constrains.AddUninitialized(ConstrainNum);

	for (int32 Y = 0; Y < YSideNum; Y++)
	{
		for (int32 X = 0; X < XSideNum; X++)
		{
			TestParticles[Y * XSideNum + X].CurPos = FVector(X * 50, Y * 50, 0);
			TestParticles[Y * XSideNum + X].OldPos = FVector(X * 50, Y * 50, 0);

			// if ((Y == 0 && X == 0) || (Y == 0 && X == XSideNum - 1))
			if(false)
			{
				TestParticles[Y * XSideNum + X].bFree = false;
			}
			else {
				TestParticles[Y * XSideNum + X].bFree = true;
			}
		}
	}

	//  @---@---@---@ constrain
	int32 XXSideConstrainNum = XSideNum - 1;
	int32 XYSideConstrainNum = YSideNum;
	for (int32 Y = 0; Y < XYSideConstrainNum; Y++)
	{
		for (int32 X = 0; X < XXSideConstrainNum; X++)
		{
			Constrains[Y * XXSideConstrainNum + X].BuildConstrain(TestParticles[Y * XSideNum + X], TestParticles[Y * XSideNum + X + 1]);
		}
	}

	//  @||@||@||@ constrain
	int32 YXSideConstrainNum = XSideNum;
	int32 YYSideConstrainNum = YSideNum - 1;
	for (int32 Y = 0; Y < YYSideConstrainNum; Y++)
	{
		for (int32 X = 0; X < YXSideConstrainNum; X++)
		{
			Constrains[Y * YXSideConstrainNum + X + XXSideConstrainNum * XYSideConstrainNum].BuildConstrain(TestParticles[Y * YXSideConstrainNum + X], TestParticles[Y * YXSideConstrainNum + X + XSideNum]);
		}
	}

	MarkRenderDynamicDataDirty();
}

void UVerletComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Velet();
	SolveConstrian();

	for (int32 i = 0; i < TestParticles.Num(); i++)
	{
		ShowPoints[i] = TestParticles[i].CurPos;
	}

	for (auto cons : Constrains)
	{
		cons.DrawDebugConstrain(GetOwner()->GetWorld());
	}

	// Need to send new data to render thread
	MarkRenderDynamicDataDirty();

	UpdateComponentToWorld();
}