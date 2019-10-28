// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementModifierPerkComponent.h"
#include "CharacterBase.h"

void UMovementModifierPerkComponent::Setup(ACharacterBase* character)
{
	if (SpeedModifier.Value != 0.0f) {
		character->StatMovementSpeed.AddModifier(SpeedModifier);
	}

	if (JumpVelocityModifier.Value != 0.0f) {
		character->StatJumpVelocity.AddModifier(JumpVelocityModifier);
	}

	if (GravityScaleModifier.Value != 0.0f) {
		character->StatGravityScale.AddModifier(GravityScaleModifier);
	}

	character->UpdateMovementProperties();
}

void UMovementModifierPerkComponent::Teardown(ACharacterBase* character)
{
	if (SpeedModifier.Value != 0.0f) {
		character->StatMovementSpeed.RemoveModifierSingle(SpeedModifier);
	}

	if (JumpVelocityModifier.Value != 0.0f) {
		character->StatJumpVelocity.RemoveModifierSingle(JumpVelocityModifier);
	}

	if (GravityScaleModifier.Value != 0.0f) {
		character->StatGravityScale.RemoveModifierSingle(GravityScaleModifier);
	}

	character->UpdateMovementProperties();
}
