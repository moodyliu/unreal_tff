#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/MeshComponent.h"
#include "VCubeComponent.generated.h"

class UStaticMesh;

struct FVParticle
{
	FVParticle() :
		NewPos(0, 0, 0),
		CurPos(0, 0, 0)
	{}



	FVector NewPos;
	FVector CurPos;
};


//This is a mesh effect component
UCLASS(hidecategories = (Object, LOD, Physics, Collision), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering, DisplayName = "VCubeComponent")
class RAYLINE_API UVCubeComponent : public UMeshComponent
{
	GENERATED_UCLASS_BODY()

public:
	//Mesh asset for this component
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = VCubeComponent)
		UStaticMesh* StaticMeshAsset;

private:

	//~ Begin UPrimitiveComponent Interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	//~ End UPrimitiveComponent Interface.

	//~ Begin UMeshComponent Interface.
	virtual int32 GetNumMaterials() const override;
	//~ End UMeshComponent Interface.

	//~ Begin USceneComponent Interface.
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	//~ Begin USceneComponent Interface.

	//~ Begin UActorComponent Interface.
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SendRenderDynamicData_Concurrent() override;
	virtual void CreateRenderState_Concurrent(FRegisterComponentContext* Context) override;
	//~ End UActorComponent Interface.

	friend class FVCubeSceneProxy;
};