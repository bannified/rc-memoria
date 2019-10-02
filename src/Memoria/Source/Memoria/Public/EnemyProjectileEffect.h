// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileEffect.h"
#include "EnemyProjectileEffect.generated.h"

class ACharacterBase;
/**
 * 
 */
UCLASS()
class MEMORIA_API UEnemyProjectileEffect : public UProjectileEffect
{
	GENERATED_BODY()
	
public:
	virtual void SetupProjectileBase(AProjectileBase* projectile) override;

	virtual void OnProjectileHit(AProjectileBase* projectile, ACharacterBase* OtherActor, const FHitResult& Hit);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnEffectActivated(AProjectileBase* projectile, ACharacterBase* OtherActor, const FHitResult& Hit);
};
