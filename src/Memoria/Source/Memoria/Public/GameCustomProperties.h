#pragma once

#include "GameCustomProperties.generated.h"

USTRUCT(BlueprintType)
struct FGameCustomProperties
{
	GENERATED_BODY()

	FGameCustomProperties() 
		: DamageMultiplier(1.0f), MoveSpeedMultiplier(1.0f), 
		SpawnCountMultiplier(1.0f), SpawnIntervalMultiplier(1.0f),
		MiniBossCountMultiplier(1.0f), ObjectiveHealthModifier(1.0f) {}

	float DamageMultiplier;
	float MoveSpeedMultiplier;

	float SpawnCountMultiplier;
	float SpawnIntervalMultiplier;

	float MiniBossCountMultiplier;

	float ObjectiveHealthModifier;

};