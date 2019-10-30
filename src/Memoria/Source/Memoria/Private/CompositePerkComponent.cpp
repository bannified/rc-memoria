// Fill out your copyright notice in the Description page of Project Settings.


#include "CompositePerkComponent.h"
#include "CharacterBase.h"

void UCompositePerkComponent::Setup(ACharacterBase* character)
{
	for (TSubclassOf<UCharacterPerkComponent> perkClass : PerkClasses)
	{
		UCharacterPerkComponent* perk = character->AddPerk(perkClass);
		perk->Setup(character);
		perkInstances.Add(perk);
	}
}

void UCompositePerkComponent::Teardown(ACharacterBase* character)
{
	for (int i = perkInstances.Num() - 1; i >= 0; i--) {
		character->RemoveAndTeardownPerk(perkInstances[i]);
		perkInstances.RemoveSingleSwap(perkInstances[i]);
	}
}
