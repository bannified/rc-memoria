// Fill out your copyright notice in the Description page of Project Settings.


#include "SuppressionGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Base.h"
#include "GenericTeamAgentInterface.h"
#include "MemoriaDeveloperSettings.h"
#include "PerpetualGameModeState.h"

void ASuppressionGameMode::StartGame()
{
	FGenericTeamId::SetAttitudeSolver(&UMemoriaDeveloperSettings::GetAttitude);
}

void ASuppressionGameMode::BeginPlay()
{
    Super::BeginPlay();
    TArray<AActor*> resultArr;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABase::StaticClass(), resultArr);

	for (AActor* actor : resultArr) {
		Objectives.Add(Cast<ABase>(actor));
	}

}
