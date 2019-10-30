// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldsPerkComponent.h"
#include "CharacterBase.h"
#include "HealthComponent.h"

void UShieldsPerkComponent::Setup(ACharacterBase* character)
{
	if (MaxShieldsModifier.Value != 0.0f) {
		character->HealthComponent->MaxShields.AddModifier(MaxShieldsModifier);
	}

	if (ShieldsRegenCooldownModifier.Value != 0.0f) {
		character->HealthComponent->ShieldRegenCooldown.AddModifier(ShieldsRegenCooldownModifier);
	}

	if (ShieldsRegenValueModifier.Value != 0.0f) {
		character->HealthComponent->ShieldRegenValue.AddModifier(ShieldsRegenValueModifier);
	}
}

void UShieldsPerkComponent::Teardown(ACharacterBase* character)
{
	if (MaxShieldsModifier.Value != 0.0f) {
		character->HealthComponent->MaxShields.RemoveModifierSingle(MaxShieldsModifier);
	}

	if (ShieldsRegenCooldownModifier.Value != 0.0f) {
		character->HealthComponent->ShieldRegenCooldown.RemoveModifierSingle(ShieldsRegenCooldownModifier);
	}

	if (ShieldsRegenValueModifier.Value != 0.0f) {
		character->HealthComponent->ShieldRegenValue.RemoveModifierSingle(ShieldsRegenValueModifier);
	}
}
