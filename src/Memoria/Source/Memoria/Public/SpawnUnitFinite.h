#pragma once

#include "SpawnUnitFinite.generated.h"

class USpawnUnitAsset;

USTRUCT(BlueprintType)
struct FSpawnUnitFinite
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnUnit")
	USpawnUnitAsset* SpawnUnitAsset;

	// Number of times the spawn unit can spawn into the level
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnUnit")
	int SpawnLimit;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnUnit")
	int SpawnGroup; //32 bits because there's no other data to pack after this
};