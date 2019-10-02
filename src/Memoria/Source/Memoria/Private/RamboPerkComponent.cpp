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
	UCharacterAttack* primary = character->GetPrimaryAttack();

	if (primary != nullptr) {
		modifiedAttack = primary;
		UProjectileAttack* projectileAttack = Cast<UProjectileAttack>(primary);
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
		UProjectileAttack* projectileAttack = Cast<UProjectileAttack>(modifiedAttack);
		if (projectileAttack) {
			projectileAttack->BulletSpreadAngle.RemoveModifierSingle(BulletSpreadModifier);
			projectileAttack->OnNormalFire.RemoveDynamic(this, &URamboPerkComponent::RepeatedFire);
		}
	}

	Super::Teardown(character);
}

void URamboPerkComponent::RepeatedFire(ACharacterBase* character, UCharacterAttack* attack)
{
	UProjectileAttack* projectileAttack = Cast<UProjectileAttack>(attack);
	if (projectileAttack) {
		projectileAttack->ComplementaryFire();
	}
}
