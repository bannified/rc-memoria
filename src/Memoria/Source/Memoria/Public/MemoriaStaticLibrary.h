// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributeModifier.h"
#include "ModifiableAttribute.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MemoriaStaticLibrary.generated.h"

class USceneComponent;

/**
 * 
 */
UCLASS()
class MEMORIA_API UMemoriaStaticLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "ModifiableAttribute")
	static void AddModifierToAttribute(UPARAM(ref) FModifiableAttribute& attribute, UPARAM(ref) const FAttributeModifier& mod);

	UFUNCTION(BlueprintCallable, Category = "ModifiableAttribute")
	static void AddModifiersToAttribute(UPARAM(ref) FModifiableAttribute& attribute, UPARAM(ref) const TArray<FAttributeModifier>& mods);

	UFUNCTION(BlueprintCallable, Category = "ModifiableAttribute")
	static void RemoveSingleModifier(UPARAM(ref) FModifiableAttribute& attribute, UPARAM(ref) const FAttributeModifier& mod);

	UFUNCTION(BlueprintCallable, Category = "ModifiableAttribute")
	static void RemoveAllWithModifier(UPARAM(ref) FModifiableAttribute& attribute, UPARAM(ref) const FAttributeModifier& mod);

	UFUNCTION(BlueprintCallable, Category = "ModifiableAttribute")
	static float GetModifiableAttributeValue(UPARAM(ref) FModifiableAttribute& attribute);

	UFUNCTION(BlueprintCallable, Category = "Actor")
	static void SetActorEnabled(UPARAM(ref) AActor* actor, bool enabled);

	UFUNCTION(BlueprintCallable, Category = "Actor")
	static void SetSceneComponentEnabled(UPARAM(ref) USceneComponent* comp, bool enabled);

};
