// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPerkComponent.h"
#include "AttributeModifier.h"
#include "BoostModifierPerkComponent.generated.h"

/**
 * 
 */
UCLASS()
class MEMORIA_API UBoostModifierPerkComponent : public UCharacterPerkComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Attack")
	FAttributeModifier BoostCooldownModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Attack")
	FAttributeModifier BoostForceModifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Attack")
	FAttributeModifier BoostAirForceModifier;

	virtual void Setup(ACharacterBase* character) override;


	virtual void Teardown(ACharacterBase* character) override;

};
