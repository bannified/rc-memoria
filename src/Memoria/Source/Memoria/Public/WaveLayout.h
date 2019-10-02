#pragma once

#include "SpawnUnit.h"
#include "WaveLayout.generated.h"


USTRUCT(BlueprintType)
struct FWaveLayout
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Description")
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Description")
	FString MainText;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	TArray< FSpawnUnit > SpawnUnits;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	float SetupTime;

};