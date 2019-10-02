// Fill out your copyright notice in the Description page of Project Settings.


#include "MemoriaStaticLibrary.h"
#include "Components/SceneComponent.h"

FORCEINLINE_DEBUGGABLE void UMemoriaStaticLibrary::AddModifierToAttribute(FModifiableAttribute& attribute, const FAttributeModifier& mod)
{
	attribute.AddModifier(mod);
}

void UMemoriaStaticLibrary::AddModifiersToAttribute(FModifiableAttribute& attribute, const TArray<FAttributeModifier>& mods)
{
	for (const FAttributeModifier& mod : mods) {
		attribute.AddModifierWithoutSort(mod);
	}

	attribute.SortMods();
}

FORCEINLINE_DEBUGGABLE void UMemoriaStaticLibrary::RemoveSingleModifier(FModifiableAttribute& attribute, const FAttributeModifier& mod)
{
	attribute.RemoveModifierSingle(mod);
}

FORCEINLINE_DEBUGGABLE void UMemoriaStaticLibrary::RemoveAllWithModifier(FModifiableAttribute& attribute, const FAttributeModifier& mod)
{
	attribute.RemoveModifiers(mod);
}

float UMemoriaStaticLibrary::GetModifiableAttributeValue(FModifiableAttribute& attribute)
{
	return attribute.GetValue();
}

FORCEINLINE_DEBUGGABLE void UMemoriaStaticLibrary::SetActorEnabled(AActor* actor, bool enabled)
{
	actor->SetActorHiddenInGame(!enabled);
	actor->SetActorEnableCollision(enabled);
	actor->SetActorTickEnabled(enabled);
}

void UMemoriaStaticLibrary::SetSceneComponentEnabled(USceneComponent* comp, bool enabled)
{
	comp->SetHiddenInGame(!enabled);

	if (enabled) {
		comp->Activate();
	}
	else {
		comp->Deactivate();
	}
}
