#pragma once

#include "SpawnUnit.generated.h"

class USpawnUnitAsset;

USTRUCT(BlueprintType)
struct FSpawnUnit
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	USpawnUnitAsset* SpawnUnitAsset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	float TimeToSpawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	int SpawnGroup; //32 bits because there's no other data to pack after this
};