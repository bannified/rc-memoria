// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiJumpPerkComponent.h"
#include "CharacterBase.h"

void UMultiJumpPerkComponent::Setup(ACharacterBase* character)
{
	// maybe check for existing MultiJumpPerkComponent? Either that or make it additive.
	m_SavedMaxJumps = character->JumpMaxCount;
	character->JumpMaxCount = MaxJumpCount;
	Super::Setup(character);
}

void UMultiJumpPerkComponent::Teardown(ACharacterBase* character)
{
	character->JumpMaxCount = m_SavedMaxJumps;
	Super::Teardown(character);
}
