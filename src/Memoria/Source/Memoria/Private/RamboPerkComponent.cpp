// Fill out your copyright notice in the Description page of Project Settings.

#include "RamboPerkComponent.h"
#include "CharacterBase.h"
#include "CharacterAttack.h"
#include "ProjectileAttack.h"

URamboPerkComponent::URamboPerkComponent()
{
	BulletSpreadModifier = FAttributeModifier(0.3f, 0, EAttributeModType::PercentAdd);
}

void URamboPerkComponent::Setup(ACharacterBase* character)
{
	ACharacterAttack* primary = character->GetPrimaryAttack();

	if (primary != nullptr) {
		modifiedAttack = primary;
		AProjectileAttack* projectileAttack = Cast<AProjectileAttack>(primary);
		if (projectileAttack) {
			projectileAttack->BulletSpreadAngle.AddModifier(BulletSpreadModifier);
			projectileAttack->OnNormalFire.AddDynamic(this, &URamboPerkComponent::RepeatedFire);
		}
	}

	Super::Setup(character);
}

void URamboPerkComponent::Teardown(ACharacterBase* character)
{
	if (modifiedAttack != nullptr) {
		AProjectileAttack* projectileAttack = Cast<AProjectileAttack>(modifiedAttack);
		if (projectileAttack) {
			projectileAttack->BulletSpreadAngle.RemoveModifierSingle(BulletSpreadModifier);
			projectileAttack->OnNormalFire.RemoveDynamic(this, &URamboPerkComponent::RepeatedFire);
		}
	}

	Super::Teardown(character);
}

void URamboPerkComponent::RepeatedFire(ACharacterBase* character, ACharacterAttack* attack)
{
	AProjectileAttack* projectileAttack = Cast<AProjectileAttack>(attack);
	if (projectileAttack) {
		projectileAttack->ComplementaryFire();
	}
}
