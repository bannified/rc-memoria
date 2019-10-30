// Fill out your copyright notice in the Description page of Project Settings.

#include "MemGameModeBase.h"
#include "SpawnPoint.h"
#include "GameModeState.h"
#include "LevelDataAsset.h"
#include "Base.h"
#include "HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "WaveDefenseGameState.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "GenericTeamAgentInterface.h"
#include "MemoriaDeveloperSettings.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SpawnUnit.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "SpawnUnitQueryInstanceWrapper.h"
#include "GameControllerBase.h"
#include "SpawnUnitAsset.h"

void AMemGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AMemGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMemGameModeBase::RegisterPlayer(APlayerController* playerController)
{
	if (playerController == nullptr) 
	{
		return;
	}

	PlayerControllerList.AddUnique(playerController);
}

AMemGameModeBase::AMemGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMemGameModeBase::MoveToState(AGameModeState* NextState)
{
	if (GameControllerInstance == nullptr) {
		return;
	}

	GameControllerInstance->MoveToState(NextState);
}

void AMemGameModeBase::StartGame(ULevelDataAsset* LevelData)
{
	FGenericTeamId::SetAttitudeSolver(&UMemoriaDeveloperSettings::GetAttitude);

	b_GameWon = false;

	if (LevelData == nullptr) {
		PRINT_INFO("MemGameModeBase %s does not have a LevelData.", *(this->GetName()));
		return;
	}

	if (GameControllerInstance != nullptr) {
		GameControllerInstance->Destroy();
	}

	if (LevelData->GameControllerClass != nullptr) {
		AGameControllerBase* gc = GetWorld()->SpawnActor<AGameControllerBase>(LevelData->GameControllerClass, FVector::ZeroVector, FRotator::ZeroRotator);
		GameControllerInstance = gc;
	}
	else if (DefaultGameControllerClass != nullptr) {
		AGameControllerBase* gc = GetWorld()->SpawnActor<AGameControllerBase>(DefaultGameControllerClass, FVector::ZeroVector, FRotator::ZeroRotator);
		GameControllerInstance = gc;
	}

	if (GameControllerInstance != nullptr) {
		GameControllerInstance->SetupWithLevelData(this, LevelData);
		LevelData->Setup(GameControllerInstance);
		GameControllerInstance->StartGame();
	}

	CurrentLevelDataAsset = LevelData;
}

void AMemGameModeBase::WinGame()
{
	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Win Game!"));

	if (!b_GameWon)
	{
		OnGameWin.Broadcast();
		b_GameWon = true;
	}
}

void AMemGameModeBase::LoseGame()
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Lose Game!"));

	OnGameLose.Broadcast();
}

void AMemGameModeBase::SetAIAttitudeSolver()
{
	FGenericTeamId::SetAttitudeSolver(&UMemoriaDeveloperSettings::GetAttitude);
}
