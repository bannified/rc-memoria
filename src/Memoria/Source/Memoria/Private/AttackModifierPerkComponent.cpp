// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackModifierPerkComponent.h"
#include "CharacterBase.h"

void UAttackModifierPerkComponent::Setup(ACharacterBase* character)
{
	if (AttackDamageModifier.Value != 0.0f) {
		character->StatBaseDamage.AddModifier(AttackDamageModifier);
	}

	if (DamageMultiplierModifier.Value != 0.0f) {
		character->StatDamageMultiplier.AddModifier(DamageMultiplierModifier);
	}

	if (AttackSpeedModifier.Value != 0.0f) {
		character->StatBaseAttackSpeed.AddModifier(AttackSpeedModifier);
	}
}

void UAttackModifierPerkComponent::Teardown(ACharacterBase* character)
{
	if (AttackDamageModifier.Value != 0.0f) {
		character->StatBaseDamage.RemoveModifierSingle(AttackDamageModifier);
	}

	if (DamageMultiplierModifier.Value != 0.0f) {
		character->StatDamageMultiplier.RemoveModifierSingle(DamageMultiplierModifier);
	}

	if (AttackSpeedModifier.Value != 0.0f) {
		character->StatBaseAttackSpeed.RemoveModifierSingle(AttackSpeedModifier);
	}
}
