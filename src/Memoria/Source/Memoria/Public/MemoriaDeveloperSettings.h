// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TeamAttitude.h"
#include "GameTeam.h"
#include "GenericTeamAgentInterface.h"
#include "MemoriaDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class MEMORIA_API UMemoriaDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "AI")
	TArray<FTeamAttitude> TeamAttitudes;

	UMemoriaDeveloperSettings(const FObjectInitializer& ObjectInitializer);

	static const UMemoriaDeveloperSettings* Get();

	UFUNCTION(BlueprintPure, Category = "AI")
	static ETeamAttitude::Type GetAttitude(FGenericTeamId Of, FGenericTeamId Towards);

};
