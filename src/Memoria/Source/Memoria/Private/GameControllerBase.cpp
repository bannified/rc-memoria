// Fill out your copyright notice in the Description page of Project Settings.

#include "GameControllerBase.h"
#include "SpawnPoint.h"
#include "GameModeState.h"
#include "LevelDataAsset.h"
#include "Base.h"
#include "HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "WaveDefenseGameState.h"
#include "SetupGameModeState.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"
#include "MemoriaDeveloperSettings.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SpawnUnit.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "SpawnUnitQueryInstanceWrapper.h"
#include "SpawnUnitAsset.h"

void AGameControllerBase::MoveToState(AGameModeState* NextState)
{

}

void AGameControllerBase::StartGame(ULevelDataAsset* LevelData)
{

}

void AGameControllerBase::NextWave()
{

}

void AGameControllerBase::WinGame()
{

}

void AGameControllerBase::LoseGame()
{

}

void AGameControllerBase::OnEnemyDestroyed(AActor* EnemyDestroyed)
{

}

void AGameControllerBase::HandleEnemySpawn(AActor* SpawnedEnemy)
{

}

void AGameControllerBase::SpawnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{

}

void AGameControllerBase::SpawnWithSpawnUnit(AMemGameModeBase* GameMode, FSpawnUnit SpawnUnit)
{

}

class ACharacterBase* AGameControllerBase::SpawnWithSpawnUnitAssetAtLocation(USpawnUnitAsset* SpawnUnitAsset, const FVector location)
{
	return nullptr;
}

void AGameControllerBase::OnEnemyDeath(AActor* Victim, AActor* Killer)
{

}

void AGameControllerBase::OnBaseTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

}

void AGameControllerBase::BeginPlay()
{

}

void AGameControllerBase::Tick(float DeltaTime)
{

}

void AGameControllerBase::SetupSpawnPointGroups()
{

}
