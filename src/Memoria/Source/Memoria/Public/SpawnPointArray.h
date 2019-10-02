#pragma once

#include "SpawnPointArray.generated.h"

class ASpawnPoint;

USTRUCT(BlueprintType)
struct FSpawnPointArray
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray< ASpawnPoint* > arr;

	ASpawnPoint* operator[] (int32 i) {
		return arr[i];
	}
};