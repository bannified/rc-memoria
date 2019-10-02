// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPerkComponent.h"
#include "MultiJumpPerkComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class MEMORIA_API UMultiJumpPerkComponent : public UCharacterPerkComponent
{
	GENERATED_BODY()

public:

	virtual void Setup(ACharacterBase* character) override;

	virtual void Teardown(ACharacterBase* character) override;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterPerk")
	uint8 MaxJumpCount;

private:
	uint8 m_SavedMaxJumps;

};
