// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VerletDraw.generated.h"

UCLASS()
class RAYLINE_API AVerletDraw : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVerletDraw();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class UVerletComponent* VerletComp;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float DebugRadius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int DebugSegments;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FColor DebugColor;
};
