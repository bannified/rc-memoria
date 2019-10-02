// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaveLayout.h"
#include "GameModeState.h"
#include "SetupGameModeState.generated.h"

class AMemGameModeBase;

class AWaveGameModeState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateEnd);

/**
 * 
 */
UCLASS()
class MEMORIA_API ASetupGameModeState : public AGameModeState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	FKey SkipButton;

	virtual void Init();

	UFUNCTION(BlueprintCallable, Category = "StateMachine")
	void InitSetupGameModeState(FWaveLayout WaveLayout, TSubclassOf<AWaveGameModeState> WaveGameModeStateClass);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float RunningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float SetupTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool fastFoward = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FWaveLayout UpcomingWave;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Events")
	FOnStateEnd OnSetupStateEnd;

	virtual void OnStateTick(AMemGameModeBase* GameMode, const float DeltaTime) override;

	virtual void OnStateExit(AMemGameModeBase* GameMode) override;

protected:
	UFUNCTION(BlueprintCallable, Category = "StateMachine")
	void TransitionToWave(AMemGameModeBase* GameMode);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	TSubclassOf<AWaveGameModeState> WaveGameModeStateClass;
	
};
