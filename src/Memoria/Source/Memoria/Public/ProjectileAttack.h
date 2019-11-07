// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAttack.h"
#include "ProjectileAttack.generated.h"

class AProjectileBase;
class USoundBase;
class UParticleSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FProjectileFiredHandler, ACharacterBase*, ownerCharacter, AActor*, targetCharacter, AProjectileBase*, spawnedProjectile);

/**
 * 
 */
UCLASS()
class MEMORIA_API AProjectileAttack : public ACharacterAttack
{
	GENERATED_BODY()
	
public:

	AProjectileAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|ProjectileAttack")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|ProjectileAttack")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|ProjectileAttack")
	FModifiableAttribute BulletSpreadAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|ProjectileAttack")
	USoundBase* ProjectileFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|ProjectileAttack")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|ProjectileAttack")
	FVector MuzzleFlashScale = FVector(1, 1, 1);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|ProjectileAttack")
	FName AnimName;

	virtual void SetupWithCharacter(ACharacterBase* character) override;

	virtual void AttackStart() override;
	virtual void AttackEnd() override;

	float LastFireTime;

	FTimerHandle TimerHandle;

	virtual void Fire();

	// Fire for free, without mana cost and without trigerring the main OnNormalFire event.
	virtual void ComplementaryFire();

	UPROPERTY(BlueprintAssignable, Category = "CharacterAttack")
	FAttackHandler OnNormalFire;
	UPROPERTY(BlueprintAssignable, Category = "CharacterAttack")
	FAttackHandler OnComplementaryFire;
	UFUNCTION(BlueprintCallable, Category = "CharacterAttack")
	virtual void PlayFireEffects();

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "CharacterAttack")
	FProjectileFiredHandler ProjectileFired;

protected:	

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterAttack")
	void ReceiveCharacterSetup(ACharacterBase* character);

	virtual float GetCooldown() override;

	AProjectileBase* m_CachedProjectileClass;

};
