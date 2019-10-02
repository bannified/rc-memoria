// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WaveLayout.h"
#include "LevelDataAsset.generated.h"

class AGameStateBase;

/**
 * 
 */
UCLASS()
class MEMORIA_API ULevelDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Waves")
	TArray< FWaveLayout > Waves;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Waves")
	FString StageName;

	UFUNCTION(BlueprintImplementableEvent)
	void Setup(AGameStateBase* GameState);
};
