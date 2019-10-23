// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileAttack.h"
#include "ChargeUpProjectileAttack.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChargeUpProjectileAttackEvent, AChargeUpProjectileAttack*, attack);

/**
 * 
 */
UCLASS()
class MEMORIA_API AChargeUpProjectileAttack : public AProjectileAttack
{
	GENERATED_BODY()
	
public:
	virtual void AttackStart() override;

	virtual void AttackEnd() override;

	UFUNCTION(BlueprintCallable, Category = "ProjectileAttack|ChargeUp")
	float GetCurrentChargeTime();
	UFUNCTION(BlueprintCallable, Category = "ProjectileAttack|ChargeUp")
	float GetMaxChargeTime();

	UFUNCTION(BlueprintCallable, Category = "ProjectileAttack|ChargeUp")
	float GetCurrentChargeProportion();

	virtual void ChargeBegin();
	virtual void ChargeReachMaxDuration();
	virtual void ChargeRelease();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ProjectileAttack|ChargeUp")
	FChargeUpProjectileAttackEvent OnChargeBegin;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ProjectileAttack|ChargeUp")
	FChargeUpProjectileAttackEvent OnChargeMax;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ProjectileAttack|ChargeUp")
	FChargeUpProjectileAttackEvent OnChargeRelease;

	UPROPERTY(BlueprintReadWrite, Category = "ProjectileAttack|ChargeUp")
	FTimerHandle ChargeUpTimerHandle;
	UPROPERTY(BlueprintReadWrite, Category = "ProjectileAttack|ChargeUp")
	FTimerHandle ChargeHoldTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|ChargeUp")
	FModifiableAttribute MaxChargeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|ChargeUp")
	FModifiableAttribute MinChargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|ChargeUp")
	FModifiableAttribute AllowedHoldTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|ChargeUp")
	FModifiableAttribute MinChargeDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|ChargeUp")
	FModifiableAttribute MaxChargeDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|ChargeUp")
	FModifiableAttribute MinChargeSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|ChargeUp")
	FModifiableAttribute MaxChargeSpeed;

	virtual void Fire() override;

	bool b_IsFullyCharged = false;

};
