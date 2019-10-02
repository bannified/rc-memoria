// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponDataAsset.h"
#include "CharacterBase.h"

bool UWeaponDataAsset::IsBuyableByCharacter(ACharacterBase* character)
{
	const int* currentLevel = character->GetWeaponCurrentLevel(this);

	int nextLevel = -1;

	if (currentLevel == nullptr) {
		nextLevel = 0;
	}
	else {
		nextLevel = *currentLevel + 1;
	}

	if (nextLevel < 0 || nextLevel >= GetMaxLevel()) {
		// invalid next level.
		return false;
	}

	// checking for price.
	FWeaponProperties properties = PropertiesList[nextLevel];

	if (character->GetCurrentMatter() < properties.CostOfUpgrade) {
		// not enough money.
		return false;
	}

	return true;
}
