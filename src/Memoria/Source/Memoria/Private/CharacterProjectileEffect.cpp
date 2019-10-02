// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterProjectileEffect.h"
#include "ProjectileBase.h"

void UCharacterProjectileEffect::SetupProjectileBase(AProjectileBase* projectile)
{
	projectile->CharacterEffects.Add(this);

	Super::SetupProjectileBase(projectile);
}

void UCharacterProjectileEffect::OnProjectileHit(AProjectileBase* projectile, ACharacterBase* OtherActor, const FHitResult& Hit)
{

	OnEffectActivated(projectile, OtherActor, Hit);
}
