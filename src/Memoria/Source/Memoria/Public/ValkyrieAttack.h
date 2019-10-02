// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAttack.h"
#include "ValkyrieAttack.generated.h"

class ABarrier;
class ACharacterBase;

/**
 * 
 */
UCLASS()
class MEMORIA_API UValkyrieAttack : public UCharacterAttack
{
	GENERATED_BODY()
	
public:
	//UValkyrieAttack();

	virtual void AttackStart() override;

	virtual void AttackEnd() override;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	float LaunchVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	float DescentForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	float LaunchDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	float FloatDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	float SpeedProportionCarryOverOnFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	float FloatGravityScale;

	virtual void SetupWithCharacter(ACharacterBase* character) override;


	virtual void TeardownWithCharacter(ACharacterBase* character) override;

protected:

	UPROPERTY(BlueprintAssignable, Category = "CharacterAttack|Valkyrie")
	FAttackHandler OnAscendStart;
	UPROPERTY(BlueprintAssignable, Category = "CharacterAttack|Valkyrie")
	FAttackHandler OnFloatStart;
	UPROPERTY(BlueprintAssignable, Category = "CharacterAttack|Valkyrie")
	FAttackHandler OnDescentStart;

	void StartAscend();

	void StartDescent();

	// Starts floating the character.
	void Float();

	FTimerHandle SharedTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	TSubclassOf<ABarrier> BarrierClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	ABarrier* BarrierInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|Valkyrie")
	float OriginalGravityScale;
};
