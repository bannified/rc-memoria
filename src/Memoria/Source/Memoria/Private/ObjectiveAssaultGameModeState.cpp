// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectiveAssaultGameModeState.h"
#include "MemGameModeBase.h"
#include "Base.h"
#include "MemoriaStaticLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameControllerBase.h"
#include "SuppressionEliminationGMS.h"
#include "SuppressionGameMode.h"

void AObjectiveAssaultGameModeState::OnStateEnter(AGameControllerBase* GameMode)
{
	this->GameModeBase = GameMode;

	for (ABase* objective : GameMode->Objectives) {
		// show objective
		objective->Reveal();
		// Deactivate shields
		UMemoriaStaticLibrary::SetSceneComponentEnabled(objective->DamageTriggerMesh, false);
		UMemoriaStaticLibrary::SetSceneComponentEnabled(objective->DamageTriggerCollider, false);
	}

	ReceiveOnStateEnter(GameMode);
}

void AObjectiveAssaultGameModeState::OnStateStart(AGameControllerBase* GameMode)
{
	ReceiveOnStateStart(GameMode);
}

void AObjectiveAssaultGameModeState::OnStateTick(AGameControllerBase* GameMode, const float DeltaTime)
{
	RunningTime += DeltaTime;

	if (RunningTime >= Lifetime) {
		// Transition to PerpetualGameModeState
		ASuppressionEliminationGMS* perpState = GetWorld()->SpawnActor<ASuppressionEliminationGMS>(SuppressionEliminationGMSClass, FVector::ZeroVector, FRotator::ZeroRotator);
		perpState->GameModeBase = GameMode;
		perpState->Init();

		GameModeBase->MoveToState(perpState);
	}
	ReceiveOnStateTick(GameMode, DeltaTime);
}

void AObjectiveAssaultGameModeState::OnStateStop(AGameControllerBase* GameMode)
{
	ReceiveOnStateStop(GameMode);
}

void AObjectiveAssaultGameModeState::OnStateExit(AGameControllerBase* GameMode)
{
	Super::OnStateEnter(GameMode);

	for (ABase* objective : GameMode->Objectives) {
		objective->Hide();
		// Reactivate shields
		UMemoriaStaticLibrary::SetSceneComponentEnabled(objective->DamageTriggerMesh, true);
		UMemoriaStaticLibrary::SetSceneComponentEnabled(objective->DamageTriggerCollider, true);
	}

	ReceiveOnStateExit(GameMode);

	Destroy();
}

void AObjectiveAssaultGameModeState::Init()
{
	ASuppressionGameMode* gm = Cast<ASuppressionGameMode>(GameModeBase);

	SuppressionEliminationGMSClass = gm->SuppressionEliminationGMSClass;
}
