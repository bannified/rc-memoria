// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAttack.h"
#include "ModifiableAttribute.h"
#include "KinesisBarrierAttack.generated.h"

class ABarrier;
class AProjectileBase;
class AController;
class UHealthComponent;

/**
 * 
 */
UCLASS()
class MEMORIA_API UKinesisBarrierAttack : public UCharacterAttack
{
	GENERATED_BODY()
	
public:
	virtual void SetupWithCharacter(ACharacterBase* ownerCharacter) override;

	virtual void TeardownWithCharacter(ACharacterBase* ownerCharacter) override;

	virtual void AttackStart() override;

	virtual void AttackEnd() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|Kinesis")
	TSubclassOf<ABarrier> BarrierClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|Kinesis")
	ABarrier* BarrierInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|Kinesis")
	TSubclassOf<AProjectileBase> OutgoingProjectileClass;

	// Multiplier applied to 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|Kinesis")
	FModifiableAttribute OutgoingDamageMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|Kinesis")
	FModifiableAttribute OutgoingProjectileSizeMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Kinesis")
	FModifiableAttribute MaxBarrierDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	FName ExitSocketName;

protected:

	UFUNCTION()
	void OnBarrierTakeDamage(UHealthComponent* HealthComp, float health, float healthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	// Damaged absorbed by the barrier. The max damage possible to be absorbed is given by the barrier's max health.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Kinesis")
	float DamageAbsorbed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Kinesis")
	FTimerHandle BarrierHoldTimeHandler;

};
