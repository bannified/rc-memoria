// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeState.h"

void AGameModeState::Init()
{

}

void AGameModeState::OnStateEnter(AGameControllerBase* GameMode)
{
    this->GameModeBase = GameMode;
	ReceiveOnStateEnter(GameMode);
}

void AGameModeState::OnStateStart(AGameControllerBase* GameMode)
{
	ReceiveOnStateStart(GameMode);
}

void AGameModeState::OnStateTick(AGameControllerBase* GameMode, float DeltaTime)
{
    ReceiveOnStateTick(GameMode, DeltaTime);
}

void AGameModeState::OnStateStop(AGameControllerBase* GameMode)
{
    ReceiveOnStateStop(GameMode);
}

void AGameModeState::OnStateExit(AGameControllerBase* GameMode)
{
    ReceiveOnStateExit(GameMode);
}
