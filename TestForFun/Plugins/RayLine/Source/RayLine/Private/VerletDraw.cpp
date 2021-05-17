// Fill out your copyright notice in the Description page of Project Settings.


#include "VerletDraw.h"
#include "VerletComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AVerletDraw::AVerletDraw()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VerletComp = CreateDefaultSubobject<UVerletComponent>(TEXT("VerletComponent"));
	VerletComp->SetupAttachment(RootComponent);
	DebugRadius = 10.f;
	DebugSegments = 10;
	DebugColor = FColor::Yellow;
}

// Called when the game starts or when spawned
void AVerletDraw::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVerletDraw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Draw FParticle
	const TArray<FVector>& ShowPoints = VerletComp->ShowPoints;
	for (const FVector ShowPoint : ShowPoints) {
		DrawDebugSphere(GetWorld(),ShowPoint,DebugRadius,DebugSegments,DebugColor);
	}

	DrawDebugSphere(GetWorld(), FVector(0,0,0), 700 + DebugRadius, 25, FColor::White);
}

