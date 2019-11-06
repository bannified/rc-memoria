// Fill out your copyright notice in the Description page of Project Settings.


#include "BoostModifierPerkComponent.h"
#include "CharacterBase.h"

void UBoostModifierPerkComponent::Setup(ACharacterBase* character)
{
	if (BoostCooldownModifier.Value != 0.0f) {
		character->Boost_Cooldown.AddModifier(BoostCooldownModifier);
	}

	if (BoostForceModifier.Value != 0.0f) {
		character->Boost_Force.AddModifier(BoostForceModifier);
	}

	if (BoostAirForceModifier.Value != 0.0f) {
		character->Boost_Air_Force.AddModifier(BoostAirForceModifier);
	}
}

void UBoostModifierPerkComponent::Teardown(ACharacterBase* character)
{
	if (BoostCooldownModifier.Value != 0.0f) {
		character->Boost_Cooldown.RemoveModifierSingle(BoostCooldownModifier);
	}

	if (BoostForceModifier.Value != 0.0f) {
		character->Boost_Force.RemoveModifierSingle(BoostForceModifier);
	}

	if (BoostAirForceModifier.Value != 0.0f) {
		character->Boost_Air_Force.RemoveModifierSingle(BoostAirForceModifier);
	}
}
