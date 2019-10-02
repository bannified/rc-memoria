// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttack.h"

void UCharacterAttack::SetupWithCharacter(ACharacterBase* character)
{
	this->ownerCharacter = character;

	OnReceiveSetupWithCharacter(character);
}

void UCharacterAttack::TeardownWithCharacter(ACharacterBase* ownerCharacter)
{

}

void UCharacterAttack::AttackStart()
{
	ReceiveAttackStart();
}

void UCharacterAttack::AttackEnd()
{
	ReceiveAttackEnd();
}

void UCharacterAttack::OffCooldown()
{
	b_OffCooldown = true;

	OnCooldownOff.Broadcast(ownerCharacter, this);
}
