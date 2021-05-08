// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnimTFFBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BPFUNCTIONFORFUN_API UAnimTFFBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Anim By Path Sync", Keywords = "Load Anim by path."), Category = "AnimBPFunctionForFun")
	static class UAnimSequence* GetAnimByPathSync(const FString& InPath);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Anim With Name", Keywords = "Set Animation Sequence variables in blueprint by Cpp."), Category = "AnimBPFunctionForFun")
	static void SetAnimByName(class UAnimInstance* AnimInstance, FName InName, const FString& InPath);
};
