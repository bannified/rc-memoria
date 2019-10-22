// Fill out your copyright notice in the Description page of Project Settings.


#include "BurstFireProjectileAttack.h"
#include "ManaComponent.h"

void ABurstFireProjectileAttack::AttackStart()
{
	ReceiveAttackStart();

	float firstDelay = FMath::Max(LastFireTime + Cooldown.GetValue() - GetWorld()->TimeSeconds, 0.00f);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABurstFireProjectileAttack::BurstFire, Cooldown.GetValue(), true, firstDelay);
	ownerCharacter->ManaComponent->InterruptReload();
}

void ABurstFireProjectileAttack::AttackEnd()
{
	ReceiveAttackEnd();
}

void ABurstFireProjectileAttack::Fire()
{
	InterBurstCount++;

	Super::Fire();

	if (InterBurstCount >= BurstCount.GetValue()) {
		GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		InterBurstCount = 0;
	}
}

void ABurstFireProjectileAttack::BurstFire()
{
	GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, &ABurstFireProjectileAttack::Fire, BurstInterval.GetValue(), true);
}