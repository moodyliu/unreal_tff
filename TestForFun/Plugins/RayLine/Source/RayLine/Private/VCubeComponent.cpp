// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved. 

#include "VCubeComponent.h"
#include "RenderingThread.h"
#include "RenderResource.h"
#include "PrimitiveViewRelevance.h"
#include "PrimitiveSceneProxy.h"
#include "VertexFactory.h"
#include "MaterialShared.h"
#include "Engine/CollisionProfile.h"
#include "Materials/Material.h"
#include "LocalVertexFactory.h"
#include "SceneManagement.h"
#include "DynamicMeshBuilder.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"

#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

/** Index Buffer */
class FVCubeIndexBuffer : public FIndexBuffer
{
public:

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		IndexBufferRHI = RHICreateIndexBuffer(sizeof(int32), NumIndices * sizeof(int32), BUF_Dynamic, CreateInfo);
	}

	int32 NumIndices;
};


/** Scene proxy */
class FVCubeSceneProxy : public FPrimitiveSceneProxy
{
public:

	SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	FVCubeSceneProxy(UVCubeComponent* Component)
		: FPrimitiveSceneProxy(Component)
		, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
		, VertexFactory(GetScene().GetFeatureLevel(), "FVCubeSceneProxy")
		, MeshAssetForRender(Component->StaticMeshAsset)
	{
		if(MeshAssetForRender){
			CaheStaticMeshAssetInfo_RenderThread(MeshAssetForRender);

			VertexBuffers.InitWithDummyData(&VertexFactory, GetRequiredVertexCount());
			IndexBuffer.NumIndices = GetRequiredIndexCount();

			const FColor VertexColor(255, 255, 255);

			// Enqueue initialization of render resource
			BeginInitResource(&IndexBuffer);

			// Grab material
			Material = Component->GetMaterial(0);
			if (Material == NULL)
			{
				Material = UMaterial::GetDefaultMaterial(MD_Surface);
			}
		}
	}

	virtual ~FVCubeSceneProxy()
	{
		VertexBuffers.PositionVertexBuffer.ReleaseResource();
		VertexBuffers.StaticMeshVertexBuffer.ReleaseResource();
		VertexBuffers.ColorVertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}

	int32 GetRequiredVertexCount() const
	{
		return CopyVertexPositionBuffer.Num();
	}

	int32 GetRequiredIndexCount() const
	{
		return CopyIndexBuffer.Num();
	}

	void CaheStaticMeshAssetInfo_RenderThread(UStaticMesh* StaticMeshData)
	{
		FStaticMeshLODResources* RenderData = &(StaticMeshData->RenderData->LODResources[0]);
		//cache the vertex data
		CopyVertexPositionBuffer.Reset();
		Vertices.Reset();
		for (int32 i = 0; i < RenderData->GetNumVertices(); i++)
		{
			FVector Position = RenderData->VertexBuffers.PositionVertexBuffer.VertexPosition(i);
			CopyVertexPositionBuffer.Add(Position);

			FDynamicMeshVertex newval;
			newval.Position = Position;
			newval.TextureCoordinate[0] = RenderData->VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(i, 0);
			newval.TangentX = RenderData->VertexBuffers.StaticMeshVertexBuffer.VertexTangentX(i);
			newval.TangentZ = RenderData->VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(i);
			Vertices.Add(newval);
		}
		//cache the index data of this mesh
		CopyIndexBuffer.Reset();
		RenderData->IndexBuffer.GetCopy(CopyIndexBuffer);

		//RenderData->VertexBuffers.StaticMeshVertexBuffer.GetTexCoordData

	}

	void BuildMesh()
	{
		if (MeshAssetForRender) {
		//Vertices.SetNumUninitialized(GetRequiredVertexCount());
		for (int i = 0; i < CopyVertexPositionBuffer.Num(); i++)
		{
			const FDynamicMeshVertex& Vertex = Vertices[i];

			VertexBuffers.PositionVertexBuffer.VertexPosition(i) = Vertex.Position;
			VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i, Vertex.TangentX.ToFVector(), Vertex.GetTangentY(), Vertex.TangentZ.ToFVector());
			VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, 0, Vertex.TextureCoordinate[0]);
			VertexBuffers.ColorVertexBuffer.VertexColor(i) = Vertex.Color;
		}

		{
			auto& VertexBuffer = VertexBuffers.PositionVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.ColorVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetTangentSize(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTangentData(), VertexBuffer.GetTangentSize());
			RHIUnlockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetTexCoordSize(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTexCoordData(), VertexBuffer.GetTexCoordSize());
			RHIUnlockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI);
		}

		void* IndexBufferData = RHILockIndexBuffer(IndexBuffer.IndexBufferRHI, 0, CopyIndexBuffer.Num() * sizeof(int32), RLM_WriteOnly);
		FMemory::Memcpy(IndexBufferData, &CopyIndexBuffer[0], CopyIndexBuffer.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(IndexBuffer.IndexBufferRHI);
		}
	}

	void SetDynamicData_RenderThread()
	{
		check(IsInRenderingThread());

		BuildMesh();
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_FVCubeSceneProxy_GetDynamicMeshElements);

		if (!MeshAssetForRender) return;

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : NULL,
			FLinearColor(0, 0.5f, 1.f)
		);

		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

		FMaterialRenderProxy* MaterialProxy = NULL;
		if (bWireframe)
		{
			MaterialProxy = WireframeMaterialInstance;
		}
		else
		{
			if(MeshAssetForRender)
				MaterialProxy = Material->GetRenderProxy();
		}

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];
				// Draw the mesh.
				FMeshBatch& Mesh = Collector.AllocateMesh();
				FMeshBatchElement& BatchElement = Mesh.Elements[0];
				BatchElement.IndexBuffer = &IndexBuffer;
				Mesh.bWireframe = bWireframe;
				Mesh.VertexFactory = &VertexFactory;
				Mesh.MaterialRenderProxy = MaterialProxy;

				bool bHasPrecomputedVolumetricLightmap;
				FMatrix PreviousLocalToWorld;
				int32 SingleCaptureIndex;
				bool bOutputVelocity;
				GetScene().GetPrimitiveUniformShaderParameters_RenderThread(GetPrimitiveSceneInfo(), bHasPrecomputedVolumetricLightmap, PreviousLocalToWorld, SingleCaptureIndex, bOutputVelocity);

				FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
				DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), PreviousLocalToWorld, GetBounds(), GetLocalBounds(), true, bHasPrecomputedVolumetricLightmap, DrawsVelocity(), bOutputVelocity);
				BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;

				// BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());
				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = GetRequiredIndexCount() / 3;
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = GetRequiredVertexCount();
				Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
				Mesh.Type = PT_TriangleList;
				Mesh.DepthPriorityGroup = SDPG_World;
				Mesh.bCanApplyViewModeOverrides = false;
				Collector.AddMesh(ViewIndex, Mesh);
			}
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;
		Result.bRenderInMainPass = ShouldRenderInMainPass();
		Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		return Result;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

private:

	UMaterialInterface* Material;
	FStaticMeshVertexBuffers VertexBuffers;
	FVCubeIndexBuffer IndexBuffer;
	FLocalVertexFactory VertexFactory;

	FMaterialRelevance MaterialRelevance;

	TArray<FVector> CopyVertexPositionBuffer;
	TArray<uint32> CopyIndexBuffer;
	TArray<FDynamicMeshVertex> Vertices;
	UStaticMesh* MeshAssetForRender;
};

//////////////////////////////////////////////////////////////////////////

UVCubeComponent::UVCubeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;

}

void UVCubeComponent::OnRegister()
{
	Super::OnRegister();

	MarkRenderDynamicDataDirty();
}

void UVCubeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Need to send new data to render thread
	MarkRenderDynamicDataDirty();

	UpdateComponentToWorld();
}

void UVCubeComponent::CreateRenderState_Concurrent(FRegisterComponentContext* Context)
{
	Super::CreateRenderState_Concurrent(Context);

	if (SceneProxy && StaticMeshAsset)
	{
		ENQUEUE_RENDER_COMMAND(FVBoxComponentDynamicData)([SceneProxy = (FVCubeSceneProxy*)SceneProxy, StaticMeshData = (UStaticMesh*)StaticMeshAsset](FRHICommandListImmediate& RHICmdList){
			SceneProxy->CaheStaticMeshAssetInfo_RenderThread(StaticMeshData);
		});
	}

	SendRenderDynamicData_Concurrent();
}

void UVCubeComponent::SendRenderDynamicData_Concurrent()
{
	if (SceneProxy)
	{
		ENQUEUE_RENDER_COMMAND(FSendRayBasicComponentDynamicData)([VBoxSceneProxy = (FVCubeSceneProxy*)SceneProxy](FRHICommandListImmediate& RHICmdList){
			VBoxSceneProxy->SetDynamicData_RenderThread();
		});
	}
}

FPrimitiveSceneProxy* UVCubeComponent::CreateSceneProxy()
{
	return new FVCubeSceneProxy(this);
}

int32 UVCubeComponent::GetNumMaterials() const
{
	return 1;
}

FBoxSphereBounds UVCubeComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds NewBounds;
	NewBounds.Origin = FVector::ZeroVector;
	NewBounds.BoxExtent = FVector(HALF_WORLD_MAX, HALF_WORLD_MAX, HALF_WORLD_MAX);
	NewBounds.SphereRadius = FMath::Sqrt(3.0f * FMath::Square(HALF_WORLD_MAX));
	return NewBounds;
}