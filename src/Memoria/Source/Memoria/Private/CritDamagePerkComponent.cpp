// Fill out your copyright notice in the Description page of Project Settings.


#include "CritDamagePerkComponent.h"
#include "CharacterBase.h"

void UCritDamagePerkComponent::Setup(ACharacterBase* character)
{
	if (CritChanceModifier.Value != 0.0f) {
		character->StatCritChance.AddModifier(CritChanceModifier);
	}

	if (CritDamageMultiplierModifier.Value != 0.0f) {
		character->StatCritDamageMultiplier.AddModifier(CritDamageMultiplierModifier);
	}
}

void UCritDamagePerkComponent::Teardown(ACharacterBase* character)
{
	if (CritChanceModifier.Value != 0.0f) {
		character->StatCritChance.RemoveModifierSingle(CritChanceModifier);
	}

	if (CritDamageMultiplierModifier.Value != 0.0f) {
		character->StatCritDamageMultiplier.RemoveModifierSingle(CritDamageMultiplierModifier);
	}
}