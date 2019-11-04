// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaModifierPerkComponent.h"
#include "CharacterBase.h"
#include "ManaComponent.h"

void UManaModifierPerkComponent::Setup(ACharacterBase* character)
{
	if (MaxManaModifier.Value != 0.0f) {
		character->ManaComponent->MaxMana.AddModifier(MaxManaModifier);
	}

	if (ReloadTimeModifier.Value != 0.0f) {
		character->ManaComponent->ReloadTime.AddModifier(ReloadTimeModifier);
	}

	character->ManaComponent->ModifyMana(character->ManaComponent->MaxMana.GetValue());
}

void UManaModifierPerkComponent::Teardown(ACharacterBase* character)
{
	if (MaxManaModifier.Value != 0.0f) {
		character->ManaComponent->MaxMana.RemoveModifierSingle(MaxManaModifier);
	}

	if (ReloadTimeModifier.Value != 0.0f) {
		character->ManaComponent->ReloadTime.RemoveModifierSingle(ReloadTimeModifier);
	}
}
