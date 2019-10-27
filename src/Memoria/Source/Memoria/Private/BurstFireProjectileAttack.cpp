// Fill out your copyright notice in the Description page of Project Settings.


#include "BurstFireProjectileAttack.h"
#include "ManaComponent.h"
#include "CharacterBase.h"

void ABurstFireProjectileAttack::AttackStart()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	ReceiveAttackStart();

	float firstDelay = FMath::Max(LastFireTime + GetCooldown() - GetWorld()->TimeSeconds, 0.00f);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABurstFireProjectileAttack::BurstFire, GetCooldown(), true, firstDelay);
	//ownerCharacter->ManaComponent->InterruptReload();
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
	Fire();
	GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, this, &ABurstFireProjectileAttack::Fire, GetBurstInterval(), true);
}

float ABurstFireProjectileAttack::GetBurstInterval()
{
	if (ownerCharacter == nullptr) {
		return BurstInterval.GetValue();
	}

	return FMath::Max(0.0f, BurstInterval.GetValue() * (1.0f / ownerCharacter->StatBaseAttackSpeed.GetValue())); // percentage based cooldown reduction
}
