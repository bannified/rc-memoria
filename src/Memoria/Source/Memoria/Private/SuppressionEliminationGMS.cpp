// Fill out your copyright notice in the Description page of Project Settings.


#include "SuppressionEliminationGMS.h"
#include "SuppressionGameMode.h"
#include "Base.h"
#include "CharacterBase.h"
#include "ObjectiveAssaultGameModeState.h"
#include "SuppressionCheckpoint.h"
#include "SuppressionGameMode.h"
#include "GameControllerBase.h"

void ASuppressionEliminationGMS::Init()
{
	ASuppressionGameMode* gm = Cast<ASuppressionGameMode>(GameModeBase);

	FSuppressionCheckpoint cp = gm->Checkpoints[gm->GetCurrentCheckpointIndex()];
	ObjectiveAssaultLifeTime = cp.ObjectiveAssaultLifeTime;
	SelectedPattern = cp.Patterns[FMath::RandRange(0, cp.Patterns.Num() - 1)];

	NextSpawnTime = FMath::FRandRange(SelectedPattern.MinSpawnInterval, SelectedPattern.MaxSpawnInterval);

	ObjectiveAssaultGameModeStateClass = cp.ObjectiveAssaultGameModeStateClass;
}

void ASuppressionEliminationGMS::OnStateEnter(AGameControllerBase* GameMode)
{
	Super::OnStateEnter(GameMode);
}

void ASuppressionEliminationGMS::OnStateStart(AGameControllerBase* GameMode)
{
	GameMode->OnEnemyUnitSpawned.AddDynamic(this, &ASuppressionEliminationGMS::HandleEnemySpawn);

	for (ABase* objective : GameMode->Objectives) {
		// Deactivate shields
		objective->Hide();
	}

	ReceiveOnStateStart(GameMode);
}

void ASuppressionEliminationGMS::OnStateTick(AGameControllerBase* GameMode, const float DeltaTime)
{
	if (SelectedPattern.FiniteSpawnUnits.Num() == 0) {
		return;
	}

	RunningTime += DeltaTime;

	if (RunningTime >= NextSpawnTime) {
		int randomIndex = FMath::RandRange(0, SelectedPattern.FiniteSpawnUnits.Num() - 1);
		FSpawnUnitFinite* suf = &(SelectedPattern.FiniteSpawnUnits[randomIndex]);

		if (suf->SpawnLimit <= 0) {
			SelectedPattern.FiniteSpawnUnits.RemoveAtSwap(randomIndex);
		}
		else {
			GameMode->SpawnWithSpawnUnit(GameMode, suf->SpawnUnit);
			suf->SpawnLimit--;

			NextSpawnTime = FMath::FRandRange(SelectedPattern.MinSpawnInterval, SelectedPattern.MaxSpawnInterval);
			RunningTime = 0;
		}
	}

	ReceiveOnStateTick(GameMode, DeltaTime);
}

void ASuppressionEliminationGMS::OnStateStop(AGameControllerBase* GameMode)
{
	Super::OnStateStop(GameMode);
}

void ASuppressionEliminationGMS::OnStateExit(AGameControllerBase* GameMode)
{
	for (ABase* objective : GameMode->Objectives) {
		// Deactivate shields
		objective->Reveal();
	}
	Super::OnStateExit(GameMode);

	Destroy();
}

void ASuppressionEliminationGMS::HandleEnemySpawn(AActor* enemy)
{
	ACharacterBase* characterBase = Cast<ACharacterBase>(enemy);

	if (characterBase == nullptr) {
		return;
	}

	characterBase->HealthComponent->OnDeathEvent.AddDynamic(this, &ASuppressionEliminationGMS::AddScoreToThreshold);
}

void ASuppressionEliminationGMS::AddScoreToThreshold(AActor* Victim, AActor* Killer)
{
	ACharacterBase* characterBase = Cast<ACharacterBase>(Victim);

	if (characterBase == nullptr) {
		return;
	}

	CurrentKillScore += characterBase->GameplayScoreValue.GetValue();

	if (CurrentKillScore >= SelectedPattern.KillScoreThreshold) {
		TransitionToAssaultState();
	}
}

void ASuppressionEliminationGMS::TransitionToAssaultState()
{
	AObjectiveAssaultGameModeState* nextState = GetWorld()->SpawnActor<AObjectiveAssaultGameModeState>(ObjectiveAssaultGameModeStateClass, FVector::ZeroVector, FRotator::ZeroRotator);
	nextState->GameModeBase = GameModeBase;
	nextState->Lifetime = ObjectiveAssaultLifeTime;
	nextState->Init();

	GameModeBase->MoveToState(nextState);
}
