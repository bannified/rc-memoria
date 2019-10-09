#pragma once

#include "SpawnUnitFinite.h"
#include "SuppressionSpawnPattern.generated.h"

USTRUCT(BlueprintType)
struct FSuppressionSpawnPattern
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnUnit")
	TArray<FSpawnUnitFinite> FiniteSpawnUnits;

	// Number of times the spawn unit can spawn into the level
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnUnit")
	float MinSpawnInterval;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnUnit")
	float MaxSpawnInterval;
};