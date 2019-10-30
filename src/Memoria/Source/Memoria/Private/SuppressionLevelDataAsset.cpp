// Fill out your copyright notice in the Description page of Project Settings.


#include "SuppressionLevelDataAsset.h"
#include "SuppressionGameMode.h"
#include "GameCustomProperties.h"
#include "Memoria.h"

USuppressionLevelDataAsset::USuppressionLevelDataAsset()
{
	GameControllerClass = ASuppressionGameMode::StaticClass();
}

void USuppressionLevelDataAsset::Setup(AGameControllerBase* controller)
{
	ASuppressionGameMode* gm = Cast<ASuppressionGameMode>(controller);

	if (gm == nullptr) {
		PRINT_INFO("Invalid GameControllerClass used in LevelDataAsset %s.", *(this->GetFullName()));
		return;
	}

	gm->Checkpoints = Checkpoints;

	// Sort in descending order
	gm->Checkpoints.Sort([](const FSuppressionCheckpoint cp1, const FSuppressionCheckpoint cp2)
	{
		return cp1.TargetLevelProgress > cp2.TargetLevelProgress;
	});

	// Set up custom properties/modifiers
	FGameCustomProperties prop = controller->CustomProperties;
	for (FSuppressionCheckpoint& cp : gm->Checkpoints)
	{
		cp.ObjectiveAssaultLifeTime *= prop.ObjectiveTimeModifier;
		if (prop.SpawnCountMultiplier == 1.0f && prop.SpawnIntervalMultiplier == 1.0f) {
			continue;
		}
		for (FSuppressionSpawnPattern& pattern : cp.Patterns) {
			pattern.MinSpawnInterval *= prop.SpawnIntervalMultiplier;
			pattern.MaxSpawnInterval *= prop.SpawnIntervalMultiplier;
			if (prop.SpawnCountMultiplier == 1.0f) {
				continue;
			}
			for (FSpawnUnitFinite& su : pattern.FiniteSpawnUnits) {
				su.SpawnLimit *= prop.SpawnCountMultiplier;
			}
		}
	}

	OnReceiveSetup(controller);
}
