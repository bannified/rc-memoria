// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectileBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "CharacterBase.h"
#include "ManaComponent.h"
#include "Memoria.h"

AProjectileAttack::AProjectileAttack()
{
	MuzzleSocketName = FName("muzzle");
}

void AProjectileAttack::SetupWithCharacter(ACharacterBase* ownerCharacter)
{
	Super::SetupWithCharacter(ownerCharacter);
	LastFireTime = -Cooldown.GetValue();

	ReceiveCharacterSetup(ownerCharacter);
}

void AProjectileAttack::AttackStart()
{
	Super::AttackStart();

	float firstDelay = FMath::Max(LastFireTime + Cooldown.GetValue() - GetWorld()->TimeSeconds, 0.00f);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectileAttack::Fire, Cooldown.GetValue(), true, firstDelay);
	ownerCharacter->ManaComponent->InterruptReload();

	ownerCharacter->bUseControllerRotationYaw = true;
}

void AProjectileAttack::AttackEnd()
{
	Super::AttackEnd();

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	ownerCharacter->bUseControllerRotationYaw = true;
}

void AProjectileAttack::Fire()
{
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

	//PlayPrimaryFireEffects();

	LastFireTime = GetWorld()->TimeSeconds;

	OnNormalFire.Broadcast(ownerCharacter, this);
}

void AProjectileAttack::ComplementaryFire()
{
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

	//PlayPrimaryFireEffects();

	LastFireTime = GetWorld()->TimeSeconds;

	OnComplementaryFire.Broadcast(ownerCharacter, this);
}
