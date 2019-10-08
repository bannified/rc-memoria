// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttack.h"

void ACharacterAttack::SetupWithCharacter(ACharacterBase* character)
{
	this->ownerCharacter = character;

	OnReceiveSetupWithCharacter(character);
}

void ACharacterAttack::TeardownWithCharacter(ACharacterBase* ownerCharacter)
{

}

void ACharacterAttack::AttackStart()
{
	ReceiveAttackStart();
}

void ACharacterAttack::AttackEnd()
{
	ReceiveAttackEnd();
}

void ACharacterAttack::OffCooldown()
{
	b_OffCooldown = true;

	OnCooldownOff.Broadcast(ownerCharacter, this);
}
