// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAttack.h"
#include "ProjectileAttack.generated.h"

class AProjectileBase;

/**
 * 
 */
UCLASS()
class MEMORIA_API UProjectileAttack : public UCharacterAttack
{
	GENERATED_BODY()
	
public:

	UProjectileAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	FModifiableAttribute BulletSpreadAngle;

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

protected:	

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterAttack")
	void ReceiveCharacterSetup(ACharacterBase* character);

	AProjectileBase* m_CachedProjectileClass;

};
