// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimTFFBlueprintFunctionLibrary.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimInstance.h"

UAnimSequence* UAnimTFFBlueprintFunctionLibrary::GetAnimByPathSync(const FString& InPath)
{
	UObject* LoadObj = StaticLoadObject(UAnimSequence::StaticClass(),nullptr,*InPath);
	if (LoadObj) {
		UAnimSequence* Seq = Cast<UAnimSequence>(LoadObj);
		if (Seq) {
			return Seq;
		}
	}
	return nullptr;
}

void UAnimTFFBlueprintFunctionLibrary::SetAnimByName(class UAnimInstance* AnimInstance, FName InName, const FString& InPath)
{
	UObjectProperty* ObjProp = FindField<UObjectProperty>(AnimInstance->GetClass(), InName);
	if (!ObjProp) {
		return;
	}
	UAnimSequence* Seq = GetAnimByPathSync(InPath);
	if(!Seq) return;
	ObjProp->SetPropertyValue_InContainer(AnimInstance,Seq);
}

UAnimTFFBlueprintFunctionLibrary::UAnimTFFBlueprintFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}