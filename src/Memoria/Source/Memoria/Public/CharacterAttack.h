// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModifiableAttribute.h"
#include "SlateBrush.h"
#include "CharacterAttack.generated.h"

class ACharacterBase;
class UBehaviorTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttackHandler, ACharacterBase*, ownerCharacter, ACharacterAttack*, attack);

UCLASS(config = Game, notplaceable, BlueprintType, Blueprintable, Transient, hideCategories = (Info, Rendering, MovementReplication, Replication, Actor), meta = (ShortTooltip = "Character Attack"))
class MEMORIA_API ACharacterAttack : public AInfo
{
	GENERATED_BODY()

public:

	// Cooldown for an attack. Can go unused.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	FModifiableAttribute Cooldown;
	
	// Mana cost for an attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	FModifiableAttribute ManaCost;

	UFUNCTION(BlueprintCallable, Category = "CharacterAttack")
	virtual void SetupWithCharacter(ACharacterBase* ownerCharacter);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterAttack")
	void OnReceiveSetupWithCharacter(ACharacterBase* characterBase);

	UFUNCTION(BlueprintCallable, Category = "CharacterAttack")
	virtual void TeardownWithCharacter(ACharacterBase* ownerCharacter);

	virtual void AttackStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterAttack")
	void ReceiveAttackStart();

	virtual void AttackEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterAttack")
	void ReceiveAttackEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	bool b_OffCooldown = true;

	UFUNCTION(BlueprintCallable, Category = "CharacterAttack")
	void OffCooldown();

	UPROPERTY(BlueprintAssignable, Category = "CharacterAttack")
	FAttackHandler OnCooldownOff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	bool HasAttackStarted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	int AttackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	FSlateBrush AttackImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|AI")
	UBehaviorTree* PreAttackBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|AI")
	UBehaviorTree* MainAttackBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|AI")
	UBehaviorTree* PostAttackBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|AI")
	FModifiableAttribute PreAttackDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|AI")
	FModifiableAttribute AttackDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack|AI")
	FModifiableAttribute PostAttackDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	bool IsInterruptable = true;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "CharacterAttack")
	FAttackHandler OnInsufficientMana;

	UFUNCTION()
	void StartReloadingOnCharacter(ACharacterBase* character, ACharacterAttack* attack);

	UFUNCTION(BlueprintCallable, Category = "CharacterAttack")
	void GoOnCooldown();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	ACharacterBase* ownerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterAttack")
	FTimerHandle CooldownTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "CharacterAttack")
	virtual float GetCooldown();
	
	UFUNCTION(BlueprintCallable, Category = "CharacterAttack")
	virtual float GetDisplayedCooldown();

	UFUNCTION(BlueprintCallable, Category = "CharacterAttack")
	virtual FTimerHandle GetDisplayedCooldownTimerHandle();

};
