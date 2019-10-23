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

class AGameControllerBase;

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

    /**
	 * State Machine
	 */

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|State Machine")
	void MoveToState(AGameModeState* NextState);

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Lifecycle")
	virtual void StartGame(ULevelDataAsset* LevelData);

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Manipulation")
	void WinGame();

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|Manipulation")
	void LoseGame();

	UFUNCTION(BlueprintCallable, Category = "MemGameModeBase|AI")
	void SetAIAttitudeSolver();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Spawning")
	ULevelDataAsset* CurrentLevelDataAsset;

    /* Events */
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameEnd OnGameWin;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameEnd OnGameLose;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "MemGameModeBase|Events")
	FOnGameEnd OnGameStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|Level")
	bool b_GameWon = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MemGameModeBase|GameController")
	AGameControllerBase* GameControllerInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MemGameModeBase|GameController")
	TSubclassOf<AGameControllerBase> DefaultGameControllerClass;

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
};
