// Fill out your copyright notice in the Description page of Project Settings.


#include "KinesisBarrierAttack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Barrier.h"
#include "ManaComponent.h"
#include "MemoriaStaticLibrary.h"
#include "ProjectileBase.h"
#include "CharacterBase.h"
#include "Memoria.h"

void AKinesisBarrierAttack::SetupWithCharacter(ACharacterBase* ownerCharacter)
{
	Super::SetupWithCharacter(ownerCharacter);

	if (BarrierClass != nullptr) {
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = ownerCharacter;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		BarrierInstance = GetWorld()->SpawnActor<ABarrier>(BarrierClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		BarrierInstance->SetOwner(ownerCharacter);
		BarrierInstance->AttachToActor(ownerCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		//	BarrierInstance->AttachToComponent(ownerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("barrier"));

		BarrierInstance->HealthComponent->OnHealthChanged.AddDynamic(this, &AKinesisBarrierAttack::OnBarrierTakeDamage);

		ownerCharacter->ActorsToIgnoreWhileAttacking.Add(BarrierInstance);

		UMemoriaStaticLibrary::SetActorEnabled(BarrierInstance, false);
	}
}

void AKinesisBarrierAttack::TeardownWithCharacter(ACharacterBase* ownerCharacter)
{
	if (ownerCharacter == nullptr) {
		return;
	}

	if (BarrierInstance != nullptr) {
		ownerCharacter->ActorsToIgnoreWhileAttacking.RemoveSingle(BarrierInstance);
		BarrierInstance->Destroy();
	}
}

void AKinesisBarrierAttack::AttackStart()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	if (ManaCost.GetValue() > ownerCharacter->ManaComponent->CurrentMana) {
		return;
	}

	// on cooldown
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle) > 0.0f) {
		return;
	}

	HasAttackStarted = true;

	ownerCharacter->ManaComponent->ModifyMana(-ManaCost.GetValue());

	b_OffCooldown = false;

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ACharacterAttack::OffCooldown, Cooldown.GetValue(), false);

	DamageAbsorbed = 0;

	BarrierInstance->HealthComponent->AlterHealth(BarrierInstance->HealthComponent->maxHealth);
	UMemoriaStaticLibrary::SetActorEnabled(BarrierInstance, true);

	GetWorld()->GetTimerManager().SetTimer(BarrierHoldTimeHandler, this, &AKinesisBarrierAttack::AttackEnd, MaxBarrierDuration.GetValue(), false);

}

void AKinesisBarrierAttack::AttackEnd()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	if (!HasAttackStarted) {
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(BarrierHoldTimeHandler);

	UMemoriaStaticLibrary::SetActorEnabled(BarrierInstance, false);

	FVector cameraLocation;
	FRotator cameraRotation;

	cameraLocation = ownerCharacter->MainCamera->GetComponentLocation();
	cameraRotation = ownerCharacter->MainCamera->GetComponentRotation();

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = ownerCharacter;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector muzzleLocation = ownerCharacter->GetWeaponMeshComponent()->GetSocketLocation(ExitSocketName);

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


	direction.Normalize();

	AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(OutgoingProjectileClass, muzzleLocation, direction.Rotation(), spawnParams);
	/* Setup projectile properties, such as Damage, TeamNumber, AOE etc */
	projectile->TeamNumber = ownerCharacter->GetTeamNumber();
	projectile->SetOwner(ownerCharacter);
	projectile->OwningController = ownerCharacter->GetController();
	projectile->OwningActor = ownerCharacter;
	projectile->ClearIgnoredActors();
	projectile->AddIgnoredActor(ownerCharacter);
	projectile->AddIgnoredActor(ownerCharacter->EquippedWeapon);
	// set projectile damage.
	projectile->DamageDealt = DamageAbsorbed * OutgoingDamageMultiplier.GetValue();
	projectile->SetActorScale3D(projectile->GetActorScale() * DamageAbsorbed * OutgoingProjectileSizeMultiplier.GetValue());

	// Shoot projectile
	HasAttackStarted = false;
}

void AKinesisBarrierAttack::OnBarrierTakeDamage(UHealthComponent* HealthComp, float health, float healthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	DamageAbsorbed += healthDelta;
}
