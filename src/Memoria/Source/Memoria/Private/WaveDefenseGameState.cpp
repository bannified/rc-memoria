// Fill out your copyright notice in the Description page of Project Settings.

#include "WaveDefenseGameState.h"
#include "PlayerControllerBase.h"
#include "PlayerStateBase.h"
#include "GameFramework/PlayerState.h"

void AWaveDefenseGameState::AddPlayerState(APlayerState* playerState)
{
	Super::AddPlayerState(playerState);

	auto playerStateBase = Cast<APlayerStateBase>(playerState);

	if (playerStateBase != nullptr && !playerStateBase->bIsInactive)
	{
		PlayerArray.AddUnique(playerStateBase);
	}
}
