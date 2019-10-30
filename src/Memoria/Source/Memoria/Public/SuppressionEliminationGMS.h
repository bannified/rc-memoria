// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeState.h"
#include "SuppressionSpawnPattern.h"
#include "SpawnUnitFinite.h"
#include "SuppressionEliminationGMS.generated.h"

class AObjectiveAssaultGameModeState;

/**
 * 
 */
UCLASS()
class MEMORIA_API ASuppressionEliminationGMS : public AGameModeState
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SuppressionEliminationGMS")
	FSuppressionSpawnPattern SelectedPattern;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SuppressionEliminationGMS")
	TSubclassOf<AObjectiveAssaultGameModeState> ObjectiveAssaultGameModeStateClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SuppressionEliminationGMS")
	float ObjectiveAssaultLifeTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SuppressionEliminationGMS")
	float NextSpawnTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SuppressionEliminationGMS")
	float RunningTime;

	virtual void Init() override;
	virtual void OnStateEnter(AGameControllerBase* GameMode) override;
	virtual void OnStateStart(AGameControllerBase* GameMode) override;
	virtual void OnStateTick(AGameControllerBase* GameMode, const float DeltaTime) override;
	virtual void OnStateStop(AGameControllerBase* GameMode) override;
	virtual void OnStateExit(AGameControllerBase* GameMode) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerpetualGameModeState")
    float CurrentKillScore;

	UFUNCTION()
	void HandleEnemySpawn(AActor* enemy);
	UFUNCTION()
	void AddScoreToThreshold(AActor* Victim, AActor* Killer);
	void TransitionToAssaultState();

};
