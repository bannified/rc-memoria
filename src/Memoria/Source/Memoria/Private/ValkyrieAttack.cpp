// Fill out your copyright notice in the Description page of Project Settings.

#include "ValkyrieAttack.h"
#include "CharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Barrier.h"
#include "ManaComponent.h"
#include "MemoriaStaticLibrary.h"

void AValkyrieAttack::AttackStart()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	if (ManaCost.GetValue() > ownerCharacter->ManaComponent->CurrentMana) {
		OnInsufficientMana.Broadcast(ownerCharacter, this);
		return;
	}

	// on cooldown
	if (GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle) > 0.0f) {
		return;
	}

	ownerCharacter->ManaComponent->ModifyMana(-ManaCost.GetValue());

	b_OffCooldown = false;

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &ACharacterAttack::OffCooldown, Cooldown.GetValue(), false);

	OriginalGravityScale = ownerCharacter->GetCharacterMovement()->GravityScale;

	StartAscend();

}

void AValkyrieAttack::AttackEnd()
{
	// Does nothing? 
}

void AValkyrieAttack::SetupWithCharacter(ACharacterBase* ownerCharacter)
{
	Super::SetupWithCharacter(ownerCharacter);

	if (ownerCharacter == nullptr) {
		return;
	}

	if (BarrierClass != nullptr) {
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = ownerCharacter;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		BarrierInstance = GetWorld()->SpawnActor<ABarrier>(BarrierClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		BarrierInstance->SetOwner(ownerCharacter);
		BarrierInstance->AttachToActor(ownerCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		ownerCharacter->ActorsToIgnoreWhileAttacking.Add(BarrierInstance);
		//	BarrierInstance->AttachToComponent(ownerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("barrier"));

		UMemoriaStaticLibrary::SetActorEnabled(BarrierInstance, false);
	}
}

void AValkyrieAttack::TeardownWithCharacter(ACharacterBase* ownerCharacter)
{
	if (ownerCharacter == nullptr) {
		return;
	}

	if (BarrierInstance != nullptr) {
		ownerCharacter->ActorsToIgnoreWhileAttacking.RemoveSingle(BarrierInstance);
		BarrierInstance->Destroy();
	}
}

void AValkyrieAttack::StartAscend()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	ownerCharacter->LaunchCharacter(FVector::UpVector * LaunchVelocity, false, false);

	// Maybe disable the character from using other attacks?

	GetWorld()->GetTimerManager().SetTimer(SharedTimerHandle, this, &AValkyrieAttack::Float, LaunchDuration, false);

	OnAscendStart.Broadcast(ownerCharacter, this);
}

void AValkyrieAttack::StartDescent()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	UCharacterMovementComponent* movementComp = ownerCharacter->GetCharacterMovement();

	movementComp->AddImpulse(FVector(0.0f, 0.0f, -DescentForce), true);
	movementComp->GravityScale = OriginalGravityScale;

	UMemoriaStaticLibrary::SetActorEnabled(BarrierInstance, false);

	OnDescentStart.Broadcast(ownerCharacter, this);
}

void AValkyrieAttack::Float()
{
	if (ownerCharacter == nullptr) {
		return;
	}

	UCharacterMovementComponent* movementComp = ownerCharacter->GetCharacterMovement();

	movementComp->Velocity.Z = SpeedProportionCarryOverOnFloat * movementComp->Velocity.Z;
	movementComp->GravityScale = FloatGravityScale;

	movementComp->UpdateComponentVelocity();

	GetWorld()->GetTimerManager().SetTimer(SharedTimerHandle, this, &AValkyrieAttack::StartDescent, FloatDuration, false);

	BarrierInstance->HealthComponent->AlterHealth(BarrierInstance->HealthComponent->maxHealth);
	UMemoriaStaticLibrary::SetActorEnabled(BarrierInstance, true);

	OnFloatStart.Broadcast(ownerCharacter, this);
}
