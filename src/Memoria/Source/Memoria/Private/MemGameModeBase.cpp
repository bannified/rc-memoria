// Fill out your copyright notice in the Description page of Project Settings.

#include "MemGameModeBase.h"
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

void AMemGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetupSpawnPointGroups();

	//AWaveDefenseGameState* gs = GetGameState<AWaveDefenseGameState>();

	//gs->SpawnUnitToLeakCountMap = TMap< USpawnUnitAsset*, int32 >();
	//gs->SpawnUnitToKillCountMap = TMap< USpawnUnitAsset*, int32 >();

	//StartGame(nullptr);

	PrimaryActorTick.bCanEverTick = true;

}

void AMemGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState != nullptr)
	{
		CurrentState->OnStateTick(this, DeltaTime);
	}

}

void AMemGameModeBase::RegisterPlayer(APlayerController* playerController)
{
	if (playerController == nullptr) 
	{
		return;
	}

	PlayerControllerList.AddUnique(playerController);
}

void AMemGameModeBase::SetupSpawnPointGroups()
{
	TArray<AActor*> spawnPoints;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), spawnPoints);

	if (spawnPoints.Num() == 0)
	{
		return;
	}

	SpawnPointsGroups.Add(FSpawnPointArray());

	for (auto s : spawnPoints)
	{
		ASpawnPoint* spawnPoint = Cast<ASpawnPoint>(s);

		SpawnPointsGroups[0].arr.Add(spawnPoint);

		for (int number : spawnPoint->GroupNumbers)
		{
			if (number == 0)
			{
				continue;
			}
			// Resize SpawnPointGroups
			if (number >= (SpawnPointsGroups.Num()))
			{
				SpawnPointsGroups.SetNum(number + 1);
			}

			SpawnPointsGroups[number].arr.Add(spawnPoint);
		}
	}
	return;
}

void AMemGameModeBase::SpawnQueryFinished(TSharedPtr<FEnvQueryResult> Result)
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

void AMemGameModeBase::SpawnWithSpawnUnit(AMemGameModeBase* GameMode, FSpawnUnit SpawnUnit)
{
    if (SpawnUnit.SpawnUnitAsset == nullptr) {
        return;
    }

    GameMode->NextSpawnUnit = SpawnUnit;

    FEnvQueryRequest request = FEnvQueryRequest(SpawnQuery, SpawnUnit.SpawnUnitAsset);
    request.SetWorldOverride(GetWorld());
    request.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &AMemGameModeBase::SpawnQueryFinished);
}

ACharacterBase* AMemGameModeBase::SpawnWithSpawnUnitAssetAtLocation(USpawnUnitAsset* SpawnUnitAsset, const FVector location)
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
        // TODO: Invoke ISpawnable (if this interface gets made)
        OnEnemyUnitSpawned.Broadcast(unit);

        return unit;
    }

    return nullptr;
}

AMemGameModeBase::AMemGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMemGameModeBase::MoveToState(AGameModeState* NextState)
{
	AGameModeState* oldState = CurrentState;

	if (CurrentState != nullptr)
	{
		CurrentState->OnStateStop(this);
		CurrentState->OnStateExit(this);
	}	

	if (NextState != nullptr)
	{
		CurrentState = NextState;

		NextState->OnStateEnter(this);
		NextState->OnStateStart(this);
	}

	OnStateChanged.Broadcast(oldState, CurrentState);
}

void AMemGameModeBase::StartGame(ULevelDataAsset* LevelData)
{
	FGenericTeamId::SetAttitudeSolver(&UMemoriaDeveloperSettings::GetAttitude);

	if (InitialStateClass) {
		AGameModeState* initialState = GetWorld()->SpawnActor<AGameModeState>(InitialStateClass, FVector::ZeroVector, FRotator::ZeroRotator);
        initialState->Init();
		MoveToState(initialState);
	}
}

void AMemGameModeBase::NextWave()
{
	CurrentWaveIndex++;

	TArray<FWaveLayout> waves = MapLevelDataAsset->Waves;

	if (CurrentWaveIndex < waves.Num())
	{
		//// Spawn Setup State
		//FActorSpawnParameters SpawnInfo;
		//SpawnInfo.Owner = this;
		//SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		//ASetupGameModeState* setupState = GetWorld()->SpawnActor<ASetupGameModeState>(SetupGameModeStateClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);

		//setupState->InitSetupGameModeState(MapLevelDataAsset->Waves[CurrentWaveIndex], WaveGameModeStateClass);

		//MoveToState(setupState);

		OnNextWave.Broadcast();
	}
	else
	{
		// TODO: no more levels left, Win!
		WinGame();
	}
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

void AMemGameModeBase::OnEnemyDestroyed(AActor* EnemyDestroyed)
{
	//AWaveDefenseGameState* gs = GetGameState<AWaveDefenseGameState>();

	//gs->EnemiesLeft--;

	EnemyDestroyed->OnDestroyed.RemoveDynamic(this, &AMemGameModeBase::OnEnemyDestroyed);
}

void AMemGameModeBase::HandleEnemySpawn(AActor* SpawnedEnemy)
{
	AWaveDefenseGameState* gs = GetGameState<AWaveDefenseGameState>();

	SpawnedEnemy->OnDestroyed.AddDynamic(this, &AMemGameModeBase::OnEnemyDestroyed);
	
	ACharacterBase* unit = Cast<ACharacterBase>(SpawnedEnemy);
	if (unit)
	{
		unit->HealthComponent->OnDeathEvent.AddDynamic(this, &AMemGameModeBase::OnEnemyDeath);
	}

	//gs->EnemiesLeft++;
}

void AMemGameModeBase::OnEnemyDeath(AActor* Victim, AActor* Killer)
{
	AWaveDefenseGameState* gs = GetGameState<AWaveDefenseGameState>();	

	if (gs)
	{
		ACharacterBase* unit = Cast<ACharacterBase>(Victim);
		ACharacterBase* character = Cast<ACharacterBase>(Killer);

		if (character && unit)
		{
			if (!gs->SpawnUnitToKillCountMap.Contains(unit->SpawnUnitAsset))
			{
				gs->SpawnUnitToKillCountMap.Add(unit->SpawnUnitAsset, 0);			
			}

			gs->SpawnUnitToKillCountMap[unit->SpawnUnitAsset]++;

			unit->HealthComponent->OnDeathEvent.RemoveDynamic(this, &AMemGameModeBase::OnEnemyDeath);
		}
	}
}

void AMemGameModeBase::OnBaseTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	AWaveDefenseGameState* gs = GetGameState<AWaveDefenseGameState>();

	if (gs)
	{
		ACharacterBase* unit = Cast<ACharacterBase>(DamageCauser);
		ABase* base = Cast<ABase>(DamagedActor);

		if (base && unit && Damage > 0)
		{
			if (!gs->SpawnUnitToLeakCountMap.Contains(unit->SpawnUnitAsset))
			{
				gs->SpawnUnitToLeakCountMap.Add(unit->SpawnUnitAsset, 0);
			}

			gs->SpawnUnitToLeakCountMap[unit->SpawnUnitAsset]++;
		}
	}
}
