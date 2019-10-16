// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "MemoriaDamageType.generated.h"

/**
 * 
 */
UCLASS()
class MEMORIA_API UMemoriaDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MemoriaDamageType")
	TSubclassOf<AActor> DamageDisplayWidgetClass;
	
};
