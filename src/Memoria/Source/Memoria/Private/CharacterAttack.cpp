// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttack.h"
#include "ManaComponent.h"
#include "CharacterBase.h"

void ACharacterAttack::SetupWithCharacter(ACharacterBase* character)
{
	this->ownerCharacter = character;

	OnInsufficientMana.AddDynamic(this, &ACharacterAttack::StartReloadingOnCharacter);

	OnReceiveSetupWithCharacter(character);
}

void ACharacterAttack::TeardownWithCharacter(ACharacterBase* ownerCharacter)
{
	OnInsufficientMana.RemoveDynamic(this, &ACharacterAttack::StartReloadingOnCharacter);
}

void ACharacterAttack::AttackStart()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	if (ownerCharacter->ManaComponent->IsReloading()) {
		return;
	}

	if (ownerCharacter->ManaComponent->CurrentMana < ManaCost.GetValue()) {
		OnInsufficientMana.Broadcast(ownerCharacter, this);
		return;
	}

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

void ACharacterAttack::StartReloadingOnCharacter(ACharacterBase* character, ACharacterAttack* attack)
{
	if (character == nullptr) {
		return;
	}

	if (character->ManaComponent == nullptr) {
		return;
	}

	character->ManaComponent->StartReload();
}

float ACharacterAttack::GetCooldown()
{
	if (ownerCharacter == nullptr) {
		return Cooldown.GetValue();
	}

	return FMath::Max(0.0f, Cooldown.GetValue() * (1.0f - ownerCharacter->StatCooldownReduction.GetValue())); // percentage based cooldown reduction
}

float ACharacterAttack::GetDisplayedCooldown()
{
	return GetCooldown();
}

FTimerHandle ACharacterAttack::GetDisplayedCooldownTimerHandle()
{
	return CooldownTimerHandle;
}
