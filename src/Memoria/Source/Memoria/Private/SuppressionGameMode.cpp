// Fill out your copyright notice in the Description page of Project Settings.


#include "SuppressionGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Base.h"
#include "GenericTeamAgentInterface.h"
#include "MemoriaDeveloperSettings.h"
#include "SuppressionEliminationGMS.h"
#include "GameModeState.h"
#include "ObjectivePoint.h"

ASuppressionGameMode::ASuppressionGameMode()
{
	SuppressionEliminationGMSClass = ASuppressionEliminationGMS::StaticClass();
}

void ASuppressionGameMode::StartGame()
{
	FGenericTeamId::SetAttitudeSolver(&UMemoriaDeveloperSettings::GetAttitude);

	TArray<AActor*> resultArr;

	// Get points where objectives can be
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectivePoint::StaticClass(), resultArr);
	ObjectivePoints.Reserve(resultArr.Num());
	for (AActor* actor : resultArr) {
		ObjectivePoints.Add(Cast<AObjectivePoint>(actor));
	}

	TArray<AActor*> resultArr2;
	// Get existing objectives
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABase::StaticClass(), resultArr2);

	for (AActor* actor : resultArr2) {
		Objectives.Add(Cast<ABase>(actor));
	}


	// Spawn objectives until NumStartingObjectives
	TArray<bool> spawned = TArray<bool>();
	spawned.SetNum(ObjectivePoints.Num());

	int i = Objectives.Num();
	for (; i < NumStartingObjectives; i++) {
		if (i >= ObjectivePoints.Num()) {
			break;
		}

		FVector finalPos = FVector::ZeroVector;
		if (ObjectivePoints.Num() > 0) {
			int selectedIndex = FMath::RandRange(0, ObjectivePoints.Num() - 1);
			while (spawned[selectedIndex] == true) {
				spawned[selectedIndex] = FMath::RandRange(0, ObjectivePoints.Num() - 1);
			}

			AObjectivePoint* point = ObjectivePoints[selectedIndex];

			if (point != nullptr) {
				finalPos = point->GetActorLocation();
				spawned[selectedIndex] = true;
			}
		}

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		params.Owner = this;

		ABase* nObjective = GetWorld()->SpawnActor<ABase>(ObjectiveClass, finalPos, FRotator::ZeroRotator, params);
		Objectives.Add(nObjective);
		nObjective->HealthComponent->DeathEvent.AddDynamic(this, &ASuppressionGameMode::CheckWinCondition);
	}

	AGameModeState* nextState = GetWorld()->SpawnActor<AGameModeState>(SuppressionEliminationGMSClass, FVector::ZeroVector, FRotator::ZeroRotator);
	nextState->GameModeBase = this;
	nextState->Init();

	MoveToState(nextState);
}

int ASuppressionGameMode::GetCurrentCheckpointIndex()
{
	float totalHp = 0;
	float currentHp = 0;

	for (int i = 0; i < Objectives.Num(); i++)
	{
		ABase* base = Objectives[i];
		totalHp += base->HealthComponent->maxHealth.GetValue();
		currentHp += base->HealthComponent->currentHealth;
	}

	float progress = currentHp / totalHp;
	int index;

	for (index = Checkpoints.Num() - 1; index >= 0; index--)
	{
		if (progress >= Checkpoints[index].TargetLevelProgress) {
			break;
		}
	}

	index = FMath::Clamp(index + 1, 0, Checkpoints.Num() - 1);

	return index;
}

void ASuppressionGameMode::CheckWinCondition()
{
	if (Objectives.Num() == 0) {
		WinGame();
		return;
	}

	for (ABase* base : Objectives) {
		if (base->HealthComponent->currentHealth > 0) {
			return;
		}
	}

	WinGame();
}

void ASuppressionGameMode::BeginPlay()
{
    Super::BeginPlay();

}
