// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModeState.h"
#include "ObjectiveAssaultGameModeState.generated.h"

class APerpetualGameModeState;

/**
 * 
 */
UCLASS()
class MEMORIA_API AObjectiveAssaultGameModeState : public AGameModeState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectiveAssault_GMS")
	float Lifetime;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ObjectiveAssault_GMS")
	float RunningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectiveAssault_GMS")
	TSubclassOf< APerpetualGameModeState > PerpetualGameModeStateClass;

	virtual void OnStateEnter(AGameControllerBase* GameMode) override;

	virtual void OnStateStart(AGameControllerBase* GameMode) override;

	virtual void OnStateTick(AGameControllerBase* GameMode, const float DeltaTime) override;

	virtual void OnStateStop(AGameControllerBase* GameMode) override;

	virtual void OnStateExit(AGameControllerBase* GameMode) override;
	
};
