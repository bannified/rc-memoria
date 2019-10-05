// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "SpawnPointArray.h"
#include "SpawnUnit.h"
#include "WaveLayout.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameControllerBase.generated.h"

class ULevelDataAsset;
class AGameModeState;
class UUserWidget;
class AWaveGameModeState;
class ASetupGameModeState;
class APlayerControllerBase;
class ABase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameAction);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChanged, AGameModeState*, OldState, AGameModeState*, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyUnitSpawn, AActor*, EnemyUnit);

/**
 * 
 */
UCLASS(config = Game, notplaceable, BlueprintType, Blueprintable, Transient, hideCategories = (Info, Rendering, MovementReplication, Replication, Actor), meta = (ShortTooltip = "GameController. Contains game's logic. can be swapped."))
class MEMORIA_API AGameControllerBase : public AInfo
{
	GENERATED_BODY()
public:
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
	FOnGameAction OnGameWin;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameAction OnGameLose;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameAction OnNextWave;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameAction OnWaveCleared;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameAction OnGameStart;

    UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnEnemyUnitSpawn OnEnemyUnitSpawned;

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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MemGameModeBase|UI")
	TSubclassOf<UUserWidget> GameHUDWidget;

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
	FOnStateChanged OnStateChanged;

};
