// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPerkComponent.h"
#include "CharacterBase.h"
#include "HealthComponent.h"

void UHealthPerkComponent::Setup(ACharacterBase* character)
{
	character->HealthComponent->maxHealth.AddModifier(HealthModifier);
	BP_Setup(character);
}

void UHealthPerkComponent::Teardown(ACharacterBase* character)
{
	character->HealthComponent->maxHealth.RemoveModifierSingle(HealthModifier);
	BP_Setup(character);
}
