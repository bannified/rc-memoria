#pragma once

#include "SpawnUnit.h"
#include "SpawnUnitFinite.generated.h"

USTRUCT(BlueprintType)
struct FSpawnUnitFinite
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnUnit")
	FSpawnUnit SpawnUnit;

	// Number of times the spawn unit can spawn into the level
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnUnit")
	int SpawnLimit;
};