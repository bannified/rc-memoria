// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WaveLayout.h"
#include "LevelDataAsset.generated.h"

class AGameStateBase;
class AGameControllerBase;

/**
 * 
 */
UCLASS()
class MEMORIA_API ULevelDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	TSubclassOf<AGameControllerBase> GameControllerClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LevelDataAsset")
	FString LevelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LevelDataAsset")
	FString LevelDescription;

	UFUNCTION(BlueprintCallable, Category = "LevelDataAsset")
	virtual void Setup(AGameControllerBase* controller);

	UFUNCTION(BlueprintImplementableEvent, Category = "LevelDataAsset")
	void OnReceiveSetup(AGameControllerBase* controller);
};
