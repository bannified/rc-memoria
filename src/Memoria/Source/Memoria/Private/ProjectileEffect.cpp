// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect.h"
#include "ProjectileBase.h"

void UProjectileEffect::SetupProjectileBase(AProjectileBase* projectile)
{
	OnProjectileBaseSetup(projectile);
}
