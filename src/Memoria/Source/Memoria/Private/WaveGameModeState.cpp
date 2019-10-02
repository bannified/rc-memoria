// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveGameModeState.h"
#include "SpawnUnit.h"
#include "SpawnUnitQueryInstanceWrapper.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnUnitAsset.h"
#include "WaveDefenseGameState.h"
#include "MemGameModeBase.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "EnvironmentQuery/EnvQueryManager.h"

void AWaveGameModeState::Init()
{
	//CurrentWaveLayout = WaveLayout;

	SpawnUnits = CurrentWaveLayout.SpawnUnits;

	Algo::Sort(SpawnUnits, [](const auto su1, const auto su2)
	{
		return su1.TimeToSpawn <= su2.TimeToSpawn;
	});
}

void AWaveGameModeState::OnStateEnter(AMemGameModeBase* GameMode)
{
	CurrentSpawnUnitIndex = 0;
	ReceiveOnStateEnter(GameMode);
}

void AWaveGameModeState::OnStateStart(AMemGameModeBase* GameMode)
{
	//WaveClearedEvent.AddDynamic(GameMode, &AMemGameModeBase::NextWave);
	//OnEnemyUnitSpawned.AddDynamic(GameMode, &AMemGameModeBase::HandleEnemySpawn);

	ReceiveOnStateStart(GameMode);
}

void AWaveGameModeState::OnStateTick(AMemGameModeBase* GameMode, const float DeltaTime)
{
	RunningTime += DeltaTime;

	if (CurrentSpawnUnitIndex < SpawnUnits.Num())
	{
		// there's still units left to spawn.
		if (RunningTime >= SpawnUnits[CurrentSpawnUnitIndex].TimeToSpawn)
		{
			GameMode->SpawnWithSpawnUnit(GameMode, SpawnUnits[CurrentSpawnUnitIndex]);
			CurrentSpawnUnitIndex++;
			if (CurrentSpawnUnitIndex >= SpawnUnits.Num())
			{
				WaveFinishSpawning.Broadcast();
			}
		}
	}
	else
	{
		// no more units left to spawn. Check for wave clear.
		AWaveDefenseGameState* gs = GameMode->GetGameState<AWaveDefenseGameState>();
		if (gs != nullptr)
		{
			if (gs->EnemiesLeft <= 0)
			{
				WaveClearedEvent.Broadcast();
			}
		}
	}

	ReceiveOnStateTick(GameMode, DeltaTime);
}

void AWaveGameModeState::OnStateStop(AMemGameModeBase* GameMode)
{
	//WaveClearedEvent.RemoveDynamic(GameMode, &AMemGameModeBase::NextWave);
	//OnEnemyUnitSpawned.RemoveDynamic(GameMode, &AMemGameModeBase::HandleEnemySpawn);

	ReceiveOnStateStop(GameMode);
}

void AWaveGameModeState::OnStateExit(AMemGameModeBase* GameMode)
{
	GameMode->OnWaveCleared.Broadcast();
	ReceiveOnStateExit(GameMode);
	Destroy();
}
