// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameTeam.h"
#include "MemoriaAIController.generated.h"

/**
 * 
 */
UCLASS()
class MEMORIA_API AMemoriaAIController : public AAIController
{
	GENERATED_BODY()

public:

	AMemoriaAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
	EGameTeam AITeamID;
	
public:

	virtual void SetGenericTeamId(const FGenericTeamId& InTeamId) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

};
