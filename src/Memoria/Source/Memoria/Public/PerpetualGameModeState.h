// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeState.h"
#include "WaveLayout.h"
#include "SpawnUnit.h"
#include "CharacterBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "PerpetualGameModeState.generated.h"

/**
 * 
 */
UCLASS()
class MEMORIA_API APerpetualGameModeState : public AGameModeState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerpetualGameModeState")
	float EndTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PerpetualGameModeState")
	float RunningTime;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PerpetualGameModeState")
    int SpawnCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerpetualGameModeState")
    int SpawnLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerpetualGameModeState")
    float KillScoreThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerpetualGameModeState")
    float CurrentKillScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PerpetualGameModeState")
	float SpawnRunningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerpetualGameModeState")
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectiveAssault_GMS")
	TSubclassOf< AObjectiveAssaultGameModeState > ObjectiveAssaultGameModeStateClass;

	virtual void Init() override;

	virtual void OnStateEnter(AMemGameModeBase* GameMode) override;

	virtual void OnStateStart(AMemGameModeBase* GameMode) override;

	virtual void OnStateTick(AMemGameModeBase* GameMode, const float DeltaTime) override;

	virtual void OnStateStop(AMemGameModeBase* GameMode) override;

	virtual void OnStateExit(AMemGameModeBase* GameMode) override;

	UFUNCTION()
	void HandleEnemySpawn(AActor* enemy);

	UFUNCTION()
	void AddScoreToThreshold(AActor* Victim, AActor* Killer);

	void TransitionToAssaultState();

	/**
	 * Wave Spawning Members
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	FWaveLayout CurrentWaveLayout;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	TArray< FSpawnUnit > SpawnUnits;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	FSpawnUnit NextSpawnUnit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 CurrentSpawnUnitIndex;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "EQS")
	UEnvQuery* SpawnQuery;

};
