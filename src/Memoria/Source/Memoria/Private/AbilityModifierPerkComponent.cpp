// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityModifierPerkComponent.h"
#include "CharacterBase.h"

void UAbilityModifierPerkComponent::Setup(ACharacterBase* character)
{
	if (CooldownReductionModifier.Value != 0.0f) {
		character->StatCooldownReduction.AddModifier(CooldownReductionModifier);
	}

	if (AbilityDamageModifier.Value != 0.0f) {
		character->StatAbilityDamage.AddModifier(AbilityDamageModifier);
	}
}

void UAbilityModifierPerkComponent::Teardown(ACharacterBase* character)
{
	if (CooldownReductionModifier.Value != 0.0f) {
		character->StatCooldownReduction.RemoveModifierSingle(CooldownReductionModifier);
	}

	if (AbilityDamageModifier.Value != 0.0f) {
		character->StatAbilityDamage.RemoveModifierSingle(AbilityDamageModifier);
	}
}
