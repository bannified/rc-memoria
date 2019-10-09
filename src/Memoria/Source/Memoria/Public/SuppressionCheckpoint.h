#pragma once

#include "SuppressionSpawnPattern.h"
#include "SuppressionCheckpoint.generated.h"

USTRUCT(BlueprintType)
struct FSuppressionCheckpoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SuppressionCheckpoint")
	TArray<FSuppressionSpawnPattern> Patterns;

	// Number of times the spawn unit can spawn into the level
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SpawnUnit")
	float TargetLevelProgress;
};