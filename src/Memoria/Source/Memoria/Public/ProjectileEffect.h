// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileEffect.generated.h"

class AProjectileBase;

/**
 * 
 */
UCLASS()
class MEMORIA_API UProjectileEffect : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FName DisplayName;

	UFUNCTION(BlueprintCallable)
	virtual void SetupProjectileBase(AProjectileBase* projectile);

	UFUNCTION(BlueprintImplementableEvent)
	void OnProjectileBaseSetup(AProjectileBase* projectile);

};
