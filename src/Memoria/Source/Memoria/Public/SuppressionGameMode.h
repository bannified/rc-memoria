// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameControllerBase.h"
#include "SuppressionGameMode.generated.h"

class APerpetualGameModeState;

/**
 * 
 */
UCLASS()
class MEMORIA_API ASuppressionGameMode : public AGameControllerBase
{
	GENERATED_BODY()

public:
	virtual void StartGame() override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

	// Assuming same setting for every perpetual state class for now
	UPROPERTY(EditDefaultsOnly, Category = "SuppressionGameMode")
	TSubclassOf< APerpetualGameModeState > PerpetualStateClass;

};
