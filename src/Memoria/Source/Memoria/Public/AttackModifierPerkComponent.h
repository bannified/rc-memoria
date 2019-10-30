// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPerkComponent.h"
#include "AttributeModifier.h"
#include "AttackModifierPerkComponent.generated.h"

/**
 * 
 */
UCLASS()
class MEMORIA_API UAttackModifierPerkComponent : public UCharacterPerkComponent
{
	GENERATED_BODY()
	
public:
	virtual void Setup(ACharacterBase* character) override;

	virtual void Teardown(ACharacterBase* character) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Attack")
	FAttributeModifier AttackDamageModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Attack")
	FAttributeModifier DamageMultiplierModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Attack")
	FAttributeModifier AttackSpeedModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Attack")
	FAttributeModifier KnockbackImpulseModifier;

};
