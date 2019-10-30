#pragma once

#include "GameCustomProperties.generated.h"

USTRUCT(BlueprintType)
struct FGameCustomProperties
{
	GENERATED_BODY()

	FGameCustomProperties() 
		: HealthMultiplier(1.0f), DamageMultiplier(1.0f), MoveSpeedMultiplier(1.0f), 
		SpawnCountMultiplier(1.0f), SpawnIntervalMultiplier(1.0f),
		MiniBossCountMultiplier(1.0f), ObjectiveHealthModifier(1.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnCountMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnIntervalMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MiniBossCountMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObjectiveHealthModifier;

};