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

	virtual void OnStateEnter(AMemGameModeBase* GameMode) override;

	virtual void OnStateStart(AMemGameModeBase* GameMode) override;

	virtual void OnStateTick(AMemGameModeBase* GameMode, const float DeltaTime) override;

	virtual void OnStateStop(AMemGameModeBase* GameMode) override;

	virtual void OnStateExit(AMemGameModeBase* GameMode) override;
	
};
