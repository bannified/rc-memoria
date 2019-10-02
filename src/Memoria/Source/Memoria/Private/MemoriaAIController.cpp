// Fill out your copyright notice in the Description page of Project Settings.


#include "MemoriaAIController.h"
#include "MemoriaPathFollowingComponent.h"

AMemoriaAIController::AMemoriaAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMemoriaPathFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void AMemoriaAIController::SetGenericTeamId(const FGenericTeamId& InTeamId)
{
	AITeamID = (EGameTeam)InTeamId.GetId();
}

FGenericTeamId AMemoriaAIController::GetGenericTeamId() const
{
	return uint8(AITeamID);
}
