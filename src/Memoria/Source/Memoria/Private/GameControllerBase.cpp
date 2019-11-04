// Fill out your copyright notice in the Description page of Project Settings.

#include "GameControllerBase.h"
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
#include "MemGameModeBase.h"
#include "SpawnUnitAsset.h"

void AGameControllerBase::BeginPlay()
{
	Super::BeginPlay();

	SetupSpawnPointGroups();

	//AWaveDefenseGameState* gs = GetGameState<AWaveDefenseGameState>();

	//gs->SpawnUnitToLeakCountMap = TMap< USpawnUnitAsset*, int32 >();
	//gs->SpawnUnitToKillCountMap = TMap< USpawnUnitAsset*, int32 >();

	//StartGame(nullptr);

	PrimaryActorTick.bCanEverTick = true;
}

void AGameControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState != nullptr) {
		CurrentState->OnStateTick(this, DeltaTime);
	}
}

void AGameControllerBase::SetupSpawnPointGroups()
{
	TArray<AActor*> spawnPoints;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), spawnPoints);

	if (spawnPoints.Num() == 0) {
		return;
	}

	SpawnPointsGroups.Add(FSpawnPointArray());

	for (auto s : spawnPoints) {
		ASpawnPoint* spawnPoint = Cast<ASpawnPoint>(s);

		SpawnPointsGroups[0].arr.Add(spawnPoint);

		for (int number : spawnPoint->GroupNumbers) {
			if (number == 0) {
				continue;
			}
			// Resize SpawnPointGroups
			if (number >= (SpawnPointsGroups.Num())) {
				SpawnPointsGroups.SetNum(number + 1);
			}

			SpawnPointsGroups[number].arr.Add(spawnPoint);
		}
	}
	return;
}

AGameControllerBase::AGameControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGameControllerBase::MoveToState(AGameModeState* NextState)
{
	AGameModeState* oldState = CurrentState;

	if (CurrentState != nullptr) {
		CurrentState->OnStateStop(this);
		CurrentState->OnStateExit(this);
	}

	if (NextState != nullptr) {
		CurrentState = NextState;

		NextState->OnStateEnter(this);
		NextState->OnStateStart(this);
	}

	OnStateChanged.Broadcast(oldState, CurrentState);
}

void AGameControllerBase::StartGame()
{
	OnReceiveStartGame();
}

void AGameControllerBase::SetupWithLevelData(AMemGameModeBase* GameMode, ULevelDataAsset* LevelData)
{
	OnReceiveSetupWithLevelData(GameMode, LevelData);
}

void AGameControllerBase::WinGame()
{
	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Win Game!"));

	if (!b_GameWon) {
		MoveToState(nullptr);
		OnGameWin.Broadcast();
		b_GameWon = true;
	}
}

void AGameControllerBase::LoseGame()
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Lose Game!"));

	OnGameLose.Broadcast();
}

void AGameControllerBase::OnEnemyDestroyed(AActor* EnemyDestroyed)
{
	//AWaveDefenseGameState* gs = GetWorld()->GetAuthGameMode()->GetGameState<AWaveDefenseGameState>();

	//gs->EnemiesLeft--;

	EnemyDestroyed->OnDestroyed.RemoveDynamic(this, &AGameControllerBase::OnEnemyDestroyed);
}

void AGameControllerBase::HandleEnemySpawn(AActor* SpawnedEnemy)
{
	AWaveDefenseGameState* gs = GetWorld()->GetAuthGameMode()->GetGameState<AWaveDefenseGameState>();

	SpawnedEnemy->OnDestroyed.AddDynamic(this, &AGameControllerBase::OnEnemyDestroyed);

	ACharacterBase* unit = Cast<ACharacterBase>(SpawnedEnemy);
	if (unit) {
		unit->HealthComponent->OnDeathEvent.AddDynamic(this, &AGameControllerBase::OnEnemyDeath);
	}

	//gs->EnemiesLeft++;
}

void AGameControllerBase::SpawnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	USpawnUnitAsset* spawnUnit = Cast<USpawnUnitAsset>(Result->Owner);
	if (spawnUnit == nullptr) {
		return;
	}

	TArray<FVector> locations;
	switch (Result->GetRawStatus()) {
		case EEnvQueryStatus::Success:
			Result->GetAllAsLocations(locations);
			if (locations.Num() > 0) {
				SpawnWithSpawnUnitAssetAtLocation(spawnUnit, locations[0]);
			}
			break;
		default:
			break;
	}
}

void AGameControllerBase::SpawnWithSpawnUnit(AGameControllerBase* GameMode, FSpawnUnit SpawnUnit)
{
	if (SpawnUnit.SpawnUnitAsset == nullptr) {
		return;
	}

	GameMode->NextSpawnUnit = SpawnUnit;

	FEnvQueryRequest request = FEnvQueryRequest(SpawnQuery, SpawnUnit.SpawnUnitAsset);
	request.SetWorldOverride(GetWorld());
	request.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &AGameControllerBase::SpawnQueryFinished);
}

class ACharacterBase* AGameControllerBase::SpawnWithSpawnUnitAssetAtLocation(USpawnUnitAsset* SpawnUnitAsset, const FVector location)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (SpawnUnitAsset == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("SpawnUnitAsset == nullptr"));
		return nullptr;
	}

	ACharacterBase* unit = GetWorld()->SpawnActor<ACharacterBase>(SpawnUnitAsset->CharacterBase, location, FRotator::ZeroRotator, SpawnInfo);
	if (unit != nullptr) {
		FVector origin, boxExtent;
		//unit->GetActorBounds(false, origin, boxExtent);
		//unit->AddActorWorldOffset(FVector(0, 0, boxExtent.Z));
		unit->SpawnUnitAsset = SpawnUnitAsset;
		SpawnUnitAsset->InitializeUnit(unit);

		unit->HealthComponent->maxHealth.AddModifier(FAttributeModifier(CustomProperties.HealthMultiplier - 1.0f, EAttributeModType::PercentAdd));
		unit->StatBaseDamage.AddModifier(FAttributeModifier(CustomProperties.DamageMultiplier - 1.0f, EAttributeModType::PercentAdd));
		unit->StatAbilityDamage.AddModifier(FAttributeModifier(CustomProperties.DamageMultiplier - 1.0f, EAttributeModType::PercentAdd));
		unit->StatMovementSpeed.AddModifier(FAttributeModifier(CustomProperties.MoveSpeedMultiplier - 1.0f, EAttributeModType::PercentAdd));
		unit->UpdateMovementProperties();
		unit->HealthComponent->FullRestoreHealthComponent();

		OnEnemyUnitSpawned.Broadcast(unit);
		return unit;
	}

	return nullptr;
}

void AGameControllerBase::OnEnemyDeath(AActor* Victim, AActor* Killer)
{
	AWaveDefenseGameState* gs = GetWorld()->GetAuthGameMode()->GetGameState<AWaveDefenseGameState>();

	if (gs) {
		ACharacterBase* unit = Cast<ACharacterBase>(Victim);
		ACharacterBase* character = Cast<ACharacterBase>(Killer);

		if (character && unit) {
			if (!gs->SpawnUnitToKillCountMap.Contains(unit->SpawnUnitAsset)) {
				gs->SpawnUnitToKillCountMap.Add(unit->SpawnUnitAsset, 0);
			}

			gs->SpawnUnitToKillCountMap[unit->SpawnUnitAsset]++;

			unit->HealthComponent->OnDeathEvent.RemoveDynamic(this, &AGameControllerBase::OnEnemyDeath);
		}
	}
}

void AGameControllerBase::OnBaseTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	AWaveDefenseGameState* gs = GetWorld()->GetAuthGameMode()->GetGameState<AWaveDefenseGameState>();

	if (gs) {
		ACharacterBase* unit = Cast<ACharacterBase>(DamageCauser);
		ABase* base = Cast<ABase>(DamagedActor);

		if (base && unit && Damage > 0) {
			if (!gs->SpawnUnitToLeakCountMap.Contains(unit->SpawnUnitAsset)) {
				gs->SpawnUnitToLeakCountMap.Add(unit->SpawnUnitAsset, 0);
			}

			gs->SpawnUnitToLeakCountMap[unit->SpawnUnitAsset]++;
		}
	}
}
