#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/MeshComponent.h"
#include "DrawDebugHelpers.h"
#include "VerletComponent.generated.h"

class UStaticMesh;

struct FParticle
{
	FParticle() :
		OldPos(0, 0, 0),
		CurPos(0, 0, 0),
		bFree(true)
	{}

	FVector OldPos;
	FVector CurPos;

	int32 bFree:1;
};

struct FVConstrain
{
	FVConstrain() {}

	FParticle* particleA;
	FParticle* particleB;

	void BuildConstrain(FParticle& A, FParticle& B)
	{
		particleA = &A;
		particleB = &B;
	}

	void SolveDistance()
	{
		if (particleA == nullptr || particleB == nullptr) return;

		float DisierDistance = 49;

		FVector Delta = particleB->CurPos - particleA->CurPos;

		float CurrentDistance = Delta.Size();
		float ErrorFactor = (CurrentDistance - DisierDistance) / CurrentDistance;

		if (particleA->bFree && particleB->bFree)
		{
			particleA->CurPos += ErrorFactor * 0.5f * Delta;
			particleB->CurPos -= ErrorFactor * 0.5f * Delta;
		}
		else if (particleA->bFree)
		{
			particleA->CurPos += ErrorFactor * Delta;
		}
		else if (particleB->bFree)
		{
			particleB->CurPos -= ErrorFactor * Delta;
		}

		//Simple collision
		if ((particleA->CurPos).Size() >= 700)
		{
			particleA->CurPos = particleA->OldPos;
		}
		if ((particleB->CurPos).Size() >= 700)
		{
			particleB->CurPos = particleB->OldPos;
		}

	}

	void DrawDebugConstrain(UWorld* world)
	{
		DrawDebugCylinder(world, particleA->CurPos, particleB->CurPos, 1.0, 6, FColor::Blue, false, -1, 0, 1.0);
	}

};

/**
 * 
 */
UCLASS(hidecategories = (Object, LOD, Physics, Collision), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering, DisplayName = "VerletComponent")
class RAYLINE_API UVerletComponent : public UMeshComponent
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = VCubeComponent)
		TArray<FVector> ShowPoints;

private:
	//------------------------------------------------------//
	TArray<FParticle> TestParticles;
	TArray<FVConstrain> Constrains;

	FVector forcedir;

	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Velet()
	{
		const float SubstepTimeSqr = 0.1;
		// Calc overall force
		//const FVector ParticleForce = FVector(0, 0, -1);

		for (int32 i = 0; i < TestParticles.Num(); i++)
		{
			if (TestParticles[i].bFree)
			{
				// Find vel
				const FVector Vel = TestParticles[i].CurPos - TestParticles[i].OldPos;
				// Update position
				const FVector NewPosition = TestParticles[i].CurPos + Vel + (SubstepTimeSqr * forcedir);
				TestParticles[i].OldPos = TestParticles[i].CurPos;
				TestParticles[i].CurPos = NewPosition;
			}
		}
	}

	void SolveDistance(FParticle& particleA, FParticle& particleB){
		float DisierDistance = 300;

		FVector Delta = particleB.CurPos - particleA.CurPos;

		float CurrentDistance = Delta.Size();
		float ErrorFactor = (CurrentDistance - DisierDistance) / CurrentDistance;

		if (particleA.bFree && particleB.bFree) {
			particleA.CurPos += ErrorFactor * .5f * Delta;
			particleB.CurPos -= ErrorFactor * .5f * Delta;
		}
		else if (particleA.bFree) {
			particleA.CurPos += ErrorFactor * Delta;
		}
		else if (particleB.bFree) {
			particleB.CurPos -= ErrorFactor * Delta;
		}

		if ((particleA.CurPos).Size() >= 8000) {
			particleA.CurPos = particleA.OldPos;
		}

		if ((particleB.CurPos).Size() >= 8000) {
			particleB.CurPos = particleB.OldPos;
		}
	}

	void SolveConstrian()
	{
		for (int32 i = 0; i < Constrains.Num(); i++)
		{
			Constrains[i].SolveDistance();
		}
	}

};
