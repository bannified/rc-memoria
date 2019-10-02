// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SlateBrush.h"
#include "SpawnUnitAsset.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class MEMORIA_API USpawnUnitAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Score Keeping")
	FName ScoreScreenDisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Score Keeping")
	int32 Score;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Score Keeping")
	FSlateBrush ScoreScreenDisplayImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gameplay")
	TSubclassOf<ACharacterBase> CharacterBase;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "UnitMod")
	void InitializeUnit(AActor* Actor);
};
