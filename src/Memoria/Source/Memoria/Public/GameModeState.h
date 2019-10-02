// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WaveLayout.h"
#include "GameModeState.generated.h"

class AMemGameModeBase;

UCLASS(config = Game, notplaceable, BlueprintType, Blueprintable, Transient, hideCategories = (Info, Rendering, MovementReplication, Replication, Actor), meta = (ShortTooltip = "Game Mode State defines the logic being carried out by the Game Mode (as a State Machine)."))
class MEMORIA_API AGameModeState : public AInfo
{
	GENERATED_BODY()

public:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
    AMemGameModeBase* GameModeBase;

	UFUNCTION(BlueprintCallable, Category = "WaveState")
	virtual void Init();

	UPROPERTY(BlueprintReadWrite, Category = "Gameplay Data")
	FName DisplayName;

	/**
	 * Called when the GameMode enters this state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Mode State")
	virtual void OnStateEnter(AMemGameModeBase* GameMode);

	/**
	 * Called whenever this states starts executing.
	 * Therefore, this method is called even when the GameMode is resumed (but still in this state).
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Mode State")
	virtual void OnStateStart(AMemGameModeBase* GameMode);

	/**
	 * Called every tick when this GameMode is active with this state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Mode State")
	virtual void OnStateTick(AMemGameModeBase* GameMode, const float DeltaTime);

	/**
	 * Called when state stops.
	 * Therefore, this method is called even when the GameMode is paused (but still in this state).
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Mode State")
	virtual void OnStateStop(AMemGameModeBase* GameMode);

	/**
	 * Called when GameMode exits this state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Game Mode State")
	virtual void OnStateExit(AMemGameModeBase* GameMode);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game Mode State")
	void ReceiveOnStateEnter(AMemGameModeBase* GameMode);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game Mode State")
	void ReceiveOnStateStart(AMemGameModeBase* GameMode);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game Mode State")
	void ReceiveOnStateTick(AMemGameModeBase* GameMode, const float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game Mode State")
	void ReceiveOnStateStop(AMemGameModeBase* GameMode);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Game Mode State")
	void ReceiveOnStateExit(AMemGameModeBase* GameMode);

};
