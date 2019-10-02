// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileEffect.h"
#include "CharacterProjectileEffect.generated.h"

class ACharacterBase;

struct FHitResult;

/**
 * 
 */
UCLASS()
class MEMORIA_API UCharacterProjectileEffect : public UProjectileEffect
{
	GENERATED_BODY()

public:
	virtual void SetupProjectileBase(AProjectileBase* projectile) override;

	virtual void OnProjectileHit(AProjectileBase* projectile, ACharacterBase* OtherActor, const FHitResult& Hit);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnEffectActivated(AProjectileBase* projectile, ACharacterBase* OtherActor, const FHitResult& Hit);

};
