// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPerkComponent.h"
#include "AttributeModifier.h"
#include "RamboPerkComponent.generated.h"

class ACharacterBase;
class UCharacterAttack;

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class MEMORIA_API URamboPerkComponent : public UCharacterPerkComponent
{
	GENERATED_BODY()

public:
	URamboPerkComponent();

	virtual void Setup(ACharacterBase* character) override;


	virtual void Teardown(ACharacterBase* character) override;

protected:
	UFUNCTION()
	void RepeatedFire(ACharacterBase* character, UCharacterAttack* attack);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerkComponent|Rambo")
	FAttributeModifier BulletSpreadModifier;

private:
	UCharacterAttack* modifiedAttack;

};
