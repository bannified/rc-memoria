// Fill out your copyright notice in the Description page of Project Settings.


#include "PerpetualGameModeState.h"
#include "SpawnUnit.h"
#include "SpawnUnitQueryInstanceWrapper.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnUnitAsset.h"
#include "WaveDefenseGameState.h"
#include "MemGameModeBase.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "ObjectiveAssaultGameModeState.h"
#include "AttributeModifier.h"
#include "ModifiableAttribute.h"
#include "Base.h"
#include "GameControllerBase.h"
#include "CharacterBase.h"

void APerpetualGameModeState::Init()
{
	SpawnUnits = CurrentWaveLayout.SpawnUnits;

	SpawnCount = 0;

	Algo::Sort(SpawnUnits, [](const auto su1, const auto su2)
	{
		return su1.TimeToSpawn <= su2.TimeToSpawn;
	});
}

void APerpetualGameModeState::OnStateEnter(AGameControllerBase* GameMode)
{
	this->GameModeBase = GameMode;
	CurrentSpawnUnitIndex = 0;
	ReceiveOnStateEnter(GameMode);
}

void APerpetualGameModeState::OnStateStart(AGameControllerBase* GameMode)
{
	//WaveClearedEvent.AddDynamic(GameMode, &AGameControllerBase::NextWave);

	GameMode->OnEnemyUnitSpawned.AddDynamic(this, &APerpetualGameModeState::HandleEnemySpawn);

	for (ABase* objective : GameMode->Objectives) {
		// Deactivate shields
		objective->HealthComponent->DamageResist.BaseValue = 1.0f;
	}

	ReceiveOnStateStart(GameMode);
}

void APerpetualGameModeState::OnStateTick(AGameControllerBase* GameMode, const float DeltaTime)
{
	RunningTime += DeltaTime;
	SpawnRunningTime += DeltaTime;

	if (SpawnCount < SpawnLimit && SpawnRunningTime >= SpawnInterval) {
		// there's still units left to spawn.
		// Spawn Random. Assume all spawn units have equal probability now.
		GameMode->SpawnWithSpawnUnit(GameMode, SpawnUnits[FMath::RandRange(0, SpawnUnits.Num() - 1)]);
		SpawnCount++;
		SpawnRunningTime = 0.0f;
	}
	else {
		// no more units left to spawn. Check for wave clear.
		//AWaveDefenseGameState* gs = GameMode->GetGameState<AWaveDefenseGameState>();
		//if (gs != nullptr) {
		//	if (gs->EnemiesLeft <= 0) {
		//		//WaveClearedEvent.Broadcast();
		//	}
		//}
	}

	ReceiveOnStateTick(GameMode, DeltaTime);
}

void APerpetualGameModeState::OnStateStop(AGameControllerBase* GameMode)
{
	//WaveClearedEvent.RemoveDynamic(GameMode, &AGameControllerBase::NextWave);

	GameMode->OnEnemyUnitSpawned.RemoveDynamic(this, &APerpetualGameModeState::HandleEnemySpawn);

	ReceiveOnStateStop(GameMode);
}

void APerpetualGameModeState::OnStateExit(AGameControllerBase* GameMode)
{

	for (ABase* objective : GameMode->Objectives) {
		// Deactivate shields
		objective->HealthComponent->DamageResist.BaseValue = 0.0f;
	}

	ReceiveOnStateExit(GameMode);
	Destroy();
}

void APerpetualGameModeState::HandleEnemySpawn(AActor* enemy)
{
	ACharacterBase* characterBase = Cast<ACharacterBase>(enemy);

	if (characterBase == nullptr) {
		return;
	}

	characterBase->HealthComponent->OnDeathEvent.AddDynamic(this, &APerpetualGameModeState::AddScoreToThreshold);
}

void APerpetualGameModeState::AddScoreToThreshold(AActor* Victim, AActor* Killer)
{
	ACharacterBase* characterBase = Cast<ACharacterBase>(Victim);

	if (characterBase == nullptr) {
		return;
	}

	CurrentKillScore += characterBase->GameplayScoreValue.GetValue();

	if (CurrentKillScore >= KillScoreThreshold) {
		TransitionToAssaultState();
	}
}

void APerpetualGameModeState::TransitionToAssaultState()
{
	AObjectiveAssaultGameModeState* nextState = GetWorld()->SpawnActor<AObjectiveAssaultGameModeState>(ObjectiveAssaultGameModeStateClass, FVector::ZeroVector, FRotator::ZeroRotator);

	nextState->Init();

	GameModeBase->MoveToState(nextState);
}
