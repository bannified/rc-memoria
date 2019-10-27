// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPerkComponent.h"
#include "AttributeModifier.h"
#include "ShieldsPerkComponent.generated.h"

/**
 * 
 */
UCLASS()
class MEMORIA_API UShieldsPerkComponent : public UCharacterPerkComponent
{
	GENERATED_BODY()
	
public:
	virtual void Setup(ACharacterBase* character) override;

	virtual void Teardown(ACharacterBase* character) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Shields")
	FAttributeModifier MaxShieldsModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Shields")
	FAttributeModifier ShieldsRegenCooldownModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Shields")
	FAttributeModifier ShieldsRegenValueModifier;
};
