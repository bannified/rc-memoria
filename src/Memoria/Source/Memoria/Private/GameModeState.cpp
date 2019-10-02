// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeState.h"

void AGameModeState::Init()
{

}

void AGameModeState::OnStateEnter(AMemGameModeBase* GameMode)
{
    this->GameModeBase = GameMode;
	ReceiveOnStateEnter(GameMode);
}

void AGameModeState::OnStateStart(AMemGameModeBase* GameMode)
{
	ReceiveOnStateStart(GameMode);
}

void AGameModeState::OnStateTick(AMemGameModeBase* GameMode, float DeltaTime)
{
    ReceiveOnStateTick(GameMode, DeltaTime);
}

void AGameModeState::OnStateStop(AMemGameModeBase* GameMode)
{
    ReceiveOnStateStop(GameMode);
}

void AGameModeState::OnStateExit(AMemGameModeBase* GameMode)
{
    ReceiveOnStateExit(GameMode);
}
