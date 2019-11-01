// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameControllerBase.h"
#include "SpawnUnitArray.h"
#include "SuppressionCheckpoint.h"
#include "SuppressionGameMode.generated.h"

class ASuppressionEliminationGMS;
class AObjectivePoint;

/**
 * 
 */
UCLASS()
class MEMORIA_API ASuppressionGameMode : public AGameControllerBase
{
	GENERATED_BODY()

public:
	ASuppressionGameMode();

	virtual void StartGame() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SuppressionGameMode")
	int NumStartingObjectives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SuppressionGameMode")
	float ObjectiveStartingHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SuppressionGameMode")
	TArray<FSuppressionCheckpoint> Checkpoints;

	// Assuming same setting for every perpetual state class for now
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SuppressionGameMode")
	TSubclassOf< ASuppressionEliminationGMS > SuppressionEliminationGMSClass;

	UFUNCTION(BlueprintCallable, Category = "SuppressionGameMode")
	int GetCurrentCheckpointIndex();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SuppressionGameMode")
	TArray<AObjectivePoint*> ObjectivePoints;

	UFUNCTION(BlueprintCallable, Category = "SuppressionGameMode")
	void CheckWinCondition();

	//UFUNCTION(BlueprintCallable, Category = "SuppressionGameMode")
	//void RandomizePositionsOfObjectives();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

};
