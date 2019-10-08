#pragma once

#include "SpawnUnitArray.generated.h"

class USpawnUnitAsset;

USTRUCT(BlueprintType)
struct FSpawnUnitArray
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray< USpawnUnitAsset* > arr;

	USpawnUnitAsset* operator[] (int32 i)
	{
		return arr[i];
	}
};