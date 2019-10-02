// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeState.h"
#include "WaveLayout.h"
#include "SpawnUnit.h"
#include "CharacterBase.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "WaveGameModeState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveCleared);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveFinishSpawning);

class USpawnUnitAsset;

/**
 * 
 */
UCLASS()
class MEMORIA_API AWaveGameModeState : public AGameModeState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float RunningTime;

	virtual void Init() override;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Events")
	FWaveCleared WaveClearedEvent;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Events")
	FWaveFinishSpawning WaveFinishSpawning;

	virtual void OnStateEnter(AMemGameModeBase* GameMode) override;

	virtual void OnStateStart(AMemGameModeBase* GameMode) override;

	virtual void OnStateTick(AMemGameModeBase* GameMode, const float DeltaTime) override;

	virtual void OnStateStop(AMemGameModeBase* GameMode) override;

	virtual void OnStateExit(AMemGameModeBase* GameMode) override;

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
};
