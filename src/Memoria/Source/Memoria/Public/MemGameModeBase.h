// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPointArray.h"
#include "SpawnUnit.h"
#include "WaveLayout.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "MemGameModeBase.generated.h"

class ULevelDataAsset;

class AGameModeState;

class UUserWidget;

class AWaveGameModeState;

class ASetupGameModeState;

class APlayerControllerBase;

class ABase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnd);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameModeStateChanged, AGameModeState*, OldState, AGameModeState*, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEnemyKilled, AActor*, Victim, AActor*, Killer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyUnitSpawned, AActor*, EnemyUnit);

/**
 * 
 */
UCLASS()
class MEMORIA_API AMemGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    AMemGameModeBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Level")
	TArray< FSpawnPointArray > SpawnPointsGroups;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Level")
    TArray<ABase*> Objectives;

    /**
	 * State Machine
	 */

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|State Machine")
	void MoveToState(AGameModeState* NextState);

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Lifecycle")
	virtual void StartGame(ULevelDataAsset* LevelData);

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Manipulation")
	void NextWave();

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Manipulation")
	void WinGame();

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Manipulation")
	void LoseGame();

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Events")
	void OnEnemyDestroyed(AActor* EnemyDestroyed);

    /* Spawning */
	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Spawning")
	virtual void HandleEnemySpawn(AActor* SpawnedEnemy);

    void SpawnQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Spawning")
	void SpawnWithSpawnUnit(AMemGameModeBase* GameMode, FSpawnUnit SpawnUnit);

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Spawning")
	class ACharacterBase* SpawnWithSpawnUnitAssetAtLocation(USpawnUnitAsset* SpawnUnitAsset, const FVector location);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MemGameModeBase|Spawning")
	UEnvQuery* SpawnQuery;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Spawning")
	FSpawnUnit NextSpawnUnit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Spawning")
	int WaveNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Spawning")
	float WaveRunningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Spawning")
	ULevelDataAsset* MapLevelDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Spawning")
	FWaveLayout CurrentWave;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Spawning")
	int CurrentSpawnUnitIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Spawning")
	int CurrentWaveIndex;

    /* Events */

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameEnd OnGameWin;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameEnd OnGameLose;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameEnd OnNextWave;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameEnd OnWaveCleared;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameEnd OnGameStart;

    UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnEnemyUnitSpawned OnEnemyUnitSpawned;

    UFUNCTION(BlueprintCallable, Category = "Events")
	virtual void OnEnemyDeath(AActor* Victim, AActor* Killer);

	UFUNCTION(BlueprintCallable, Category = "Events")
	void OnBaseTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MemGameModeBase|Level")
	bool b_GameWon = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MemGameModeBase|Lifecycle")
	TSubclassOf<AGameModeState> InitialStateClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "MemGameModeBase|Player Management")
	TArray<APlayerController*> PlayerControllerList;

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Player Management")
	virtual void RegisterPlayer(APlayerController* playerController);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MemGameModeBase|UI")
	TSubclassOf<UUserWidget> GameHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|State")
	bool b_isWaveOngoing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Lifecycle")
	ULevelDataAsset* FallbackLevelData;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetupSpawnPointGroups();

	/**
	 * State Machine
	 */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|State Machine")
	AGameModeState* SavedState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|State Machine")
	AGameModeState* CurrentState;

	UPROPERTY(BlueprintAssignable, Category = "MemGameModeBase|State Machine")
	FOnGameModeStateChanged OnStateChanged;

};
