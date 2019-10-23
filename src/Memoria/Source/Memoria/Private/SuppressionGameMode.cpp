// Fill out your copyright notice in the Description page of Project Settings.


#include "SuppressionGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Base.h"
#include "GenericTeamAgentInterface.h"
#include "MemoriaDeveloperSettings.h"
#include "SuppressionEliminationGMS.h"
#include "GameModeState.h"

ASuppressionGameMode::ASuppressionGameMode()
{
	SuppressionEliminationGMSClass = ASuppressionEliminationGMS::StaticClass();
}

void ASuppressionGameMode::StartGame()
{
	FGenericTeamId::SetAttitudeSolver(&UMemoriaDeveloperSettings::GetAttitude);

	TArray<AActor*> resultArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABase::StaticClass(), resultArr);

	for (AActor* actor : resultArr) {
		Objectives.Add(Cast<ABase>(actor));
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
		totalHp += base->HealthComponent->maxHealth;
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

void ASuppressionGameMode::BeginPlay()
{
    Super::BeginPlay();

}
