// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargeUpProjectileAttack.h"
#include "ManaComponent.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

void AChargeUpProjectileAttack::AttackStart()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	if (ownerCharacter->ManaComponent->CurrentMana < ManaCost.GetValue()) {
		return;
	}

	// on cooldown
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle) > 0.0f) {
		return;
	}

	float firstDelay = FMath::Max(LastFireTime + Cooldown.GetValue() - GetWorld()->TimeSeconds, 0.00f);

	ownerCharacter->ManaComponent->InterruptReload();

	ChargeBegin();

	ReceiveAttackStart();
}

void AChargeUpProjectileAttack::AttackEnd()
{

	ChargeRelease();

	ReceiveAttackEnd();
}

float AChargeUpProjectileAttack::GetCurrentChargeTime()
{
	return GetWorld()->GetTimerManager().GetTimerElapsed(ChargeUpTimerHandle);
}

float AChargeUpProjectileAttack::GetMaxChargeTime()
{
	return GetWorld()->GetTimerManager().GetTimerRate(ChargeUpTimerHandle);
}

float AChargeUpProjectileAttack::GetCurrentChargeProportion()
{
	if (b_IsFullyCharged) {
		return 1.0f;
	}

	return GetCurrentChargeTime() / GetMaxChargeTime();
}

void AChargeUpProjectileAttack::ChargeBegin()
{
	b_IsFullyCharged = false;

	GetWorld()->GetTimerManager().SetTimer(ChargeUpTimerHandle, this, &AChargeUpProjectileAttack::ChargeReachMaxDuration, MaxChargeTime.GetValue(), false);
	OnChargeBegin.Broadcast(this);
}

void AChargeUpProjectileAttack::ChargeReachMaxDuration()
{
	b_IsFullyCharged = true;

	if (AllowedHoldTime.GetValue() > 0) {
		GetWorld()->GetTimerManager().SetTimer(ChargeHoldTimerHandle, this, &AChargeUpProjectileAttack::ChargeRelease, AllowedHoldTime.GetValue(), false);
	}
	OnChargeMax.Broadcast(this);
}

void AChargeUpProjectileAttack::ChargeRelease()
{
	if (b_IsFullyCharged || GetCurrentChargeTime() > MinChargeTime.GetValue()) {
		Fire();
		GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ACharacterAttack::OffCooldown, Cooldown.GetValue(), false);
	}

	b_IsFullyCharged = false;

	OnChargeRelease.Broadcast(this);

	GetWorld()->GetTimerManager().ClearTimer(ChargeUpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ChargeHoldTimerHandle);
}

void AChargeUpProjectileAttack::Fire()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	if (ownerCharacter->ManaComponent->CurrentMana < ManaCost.GetValue()) {
		return;
	}

	ownerCharacter->ManaComponent->ModifyMana(-ManaCost.GetValue());

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ACharacterAttack::OffCooldown, Cooldown.GetValue(), false);

	FVector cameraLocation;
	FRotator cameraRotation;

	cameraLocation = ownerCharacter->MainCamera->GetComponentLocation();
	cameraRotation = ownerCharacter->MainCamera->GetComponentRotation();

	// bullet spread
	float halfRad = FMath::DegreesToRadians(BulletSpreadAngle.GetValue());

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = ownerCharacter;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector muzzleLocation;

	if (ownerCharacter->GetWeaponMeshComponent() == nullptr) {
		muzzleLocation = ownerCharacter->GetActorLocation();
	}
	else {
		muzzleLocation = ownerCharacter->GetWeaponMeshComponent()->GetSocketLocation(MuzzleSocketName);
	}

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(ownerCharacter);
	queryParams.AddIgnoredActor(ownerCharacter->EquippedWeapon);
	queryParams.bReturnPhysicalMaterial = true; // returns material that it hits (headshots, bodyshots, etc)
	queryParams.bTraceComplex = true;

	FHitResult result;

	FVector cameraViewVector = cameraLocation + cameraRotation.Vector() * 10000;

	FVector direction = cameraViewVector - muzzleLocation;

	if (GetWorld()->LineTraceSingleByChannel(result, cameraLocation, cameraViewVector, COLLISION_AIM_DETECT, queryParams)) {
		direction = result.ImpactPoint - muzzleLocation;

		// ensure that direction within 90 degrees.
		if (FVector::DotProduct(direction, cameraViewVector) < 0) {
			direction = cameraViewVector - muzzleLocation;
		}
	}

	direction = FMath::VRandCone(direction, halfRad, halfRad);

	direction.Normalize();

	AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, muzzleLocation, direction.Rotation(), spawnParams);
	/* Setup projectile properties, such as Damage, TeamNumber, AOE etc */
	projectile->TeamNumber = ownerCharacter->GetTeamNumber();
	projectile->SetOwner(ownerCharacter);
	projectile->OwningController = ownerCharacter->GetController();
	projectile->OwningActor = ownerCharacter;
	projectile->ClearIgnoredActors();
	projectile->AddIgnoredActor(ownerCharacter);
	projectile->AddIgnoredActor(ownerCharacter->EquippedWeapon);

	for (AActor* actor : ownerCharacter->ActorsToIgnoreWhileAttacking) {
		projectile->AddIgnoredActor(actor);
	}
	// set projectile damage.

	float prop = GetCurrentChargeProportion();

	projectile->DamageDealt = FMath::Lerp(MinChargeDamage.GetValue(), MaxChargeDamage.GetValue(), prop);
	projectile->ProjectileMovement->InitialSpeed = FMath::Lerp(MinChargeDamage.GetValue(), MaxChargeDamage.GetValue(), prop);

	//PlayPrimaryFireEffects();

	LastFireTime = GetWorld()->TimeSeconds;

	OnNormalFire.Broadcast(ownerCharacter, this);
}
