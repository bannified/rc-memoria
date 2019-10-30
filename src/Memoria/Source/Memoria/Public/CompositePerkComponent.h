// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterPerkComponent.h"
#include "CompositePerkComponent.generated.h"

class UCharacterPerkComponent;

/**
 * 
 */
UCLASS()
class MEMORIA_API UCompositePerkComponent : public UCharacterPerkComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perks|Composite")
	TArray< TSubclassOf<UCharacterPerkComponent> > PerkClasses;

	virtual void Setup(ACharacterBase* character) override;

	virtual void Teardown(ACharacterBase* character) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perks|Composite")
	TArray< UCharacterPerkComponent* > perkInstances;

};
