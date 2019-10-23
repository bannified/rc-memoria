// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileAttack.h"
#include "BurstFireProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class MEMORIA_API ABurstFireProjectileAttack : public AProjectileAttack
{
	GENERATED_BODY()

public:
	virtual void AttackStart() override;
	virtual void AttackEnd() override;

	virtual void Fire() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|BurstFire")
	FTimerHandle BurstTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|BurstFire")
	FModifiableAttribute BurstCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|BurstFire")
	FModifiableAttribute BurstInterval;

	UFUNCTION(BlueprintCallable, Category = "ProjectileAttack|BurstFire")
	void BurstFire();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProjectileAttack|BurstFire")
	int InterBurstCount;

};
