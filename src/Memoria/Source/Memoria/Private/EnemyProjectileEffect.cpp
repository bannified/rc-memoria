// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyProjectileEffect.h"
#include "ProjectileBase.h"

void UEnemyProjectileEffect::SetupProjectileBase(AProjectileBase* projectile)
{
	projectile->UnitEffects.Add(this);

	Super::SetupProjectileBase(projectile);
}

void UEnemyProjectileEffect::OnProjectileHit(AProjectileBase* projectile, ACharacterBase* OtherActor, const FHitResult& Hit)
{
	OnEffectActivated(projectile, OtherActor, Hit);
}
