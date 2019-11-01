// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelDataAsset.h"
#include "SuppressionCheckpoint.h"
#include "SuppressionLevelDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MEMORIA_API USuppressionLevelDataAsset : public ULevelDataAsset
{
	GENERATED_BODY()
	
public:

	USuppressionLevelDataAsset();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Suppression")
	int NumOfObjectives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Suppression")
	float ObjectiveStartingHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Suppression")
	TArray<FSuppressionCheckpoint> Checkpoints;

	virtual void Setup(AGameControllerBase* controller) override;

};
