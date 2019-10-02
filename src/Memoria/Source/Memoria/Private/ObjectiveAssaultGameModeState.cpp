// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectiveAssaultGameModeState.h"
#include "MemGameModeBase.h"
#include "Base.h"
#include "MemoriaStaticLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PerpetualGameModeState.h"

void AObjectiveAssaultGameModeState::OnStateEnter(AMemGameModeBase* GameMode)
{
	this->GameModeBase = GameMode;

	for (ABase* objective : GameMode->Objectives) {
		// Deactivate shields
		UMemoriaStaticLibrary::SetSceneComponentEnabled(objective->DamageTriggerMesh, false);
		UMemoriaStaticLibrary::SetSceneComponentEnabled(objective->DamageTriggerCollider, false);
	}

	ReceiveOnStateEnter(GameMode);
}

void AObjectiveAssaultGameModeState::OnStateStart(AMemGameModeBase* GameMode)
{

}

void AObjectiveAssaultGameModeState::OnStateTick(AMemGameModeBase* GameMode, const float DeltaTime)
{
	RunningTime += DeltaTime;

	if (RunningTime >= Lifetime) {
		// Transition to PerpetualGameModeState
		APerpetualGameModeState* perpState = GetWorld()->SpawnActor<APerpetualGameModeState>(PerpetualGameModeStateClass, FVector::ZeroVector, FRotator::ZeroRotator);
		
		perpState->CurrentWaveLayout = GameMode->CurrentWave;
		perpState->Init();

		GameModeBase->MoveToState(perpState);
	}

}

void AObjectiveAssaultGameModeState::OnStateStop(AMemGameModeBase* GameMode)
{

}

void AObjectiveAssaultGameModeState::OnStateExit(AMemGameModeBase* GameMode)
{
	Super::OnStateEnter(GameMode);

	for (ABase* objective : GameMode->Objectives) {
		// Reactivate shields
		UMemoriaStaticLibrary::SetSceneComponentEnabled(objective->DamageTriggerMesh, true);
		UMemoriaStaticLibrary::SetSceneComponentEnabled(objective->DamageTriggerCollider, true);
	}

}