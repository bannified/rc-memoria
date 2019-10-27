// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModifiableAttribute.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, AActor*, Victim, AActor*, Killer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComp, float, health, float, healthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEMORIA_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Runtime")
	FModifiableAttribute DamageResist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Runtime")
	uint8 TeamNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Gameplay")
	FModifiableAttribute maxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HealthComponent|Runtime")
	float currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Runtime")
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Gameplay")
	float startingHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Gameplay")
	float StartingShields;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HealthComponent|Runtime")
	float CurrentShields;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Gameplay")
	FModifiableAttribute MaxShields;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Gameplay")
	float ShieldsInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Gameplay")
	FModifiableAttribute ShieldRegenValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Gameplay")
	FModifiableAttribute ShieldRegenCooldown;

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FDeathEvent DeathEvent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	

	float GetHealth() const;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Initialize health with specified value
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void Init();

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void FullRestoreHealthComponent();

	/** Changes the health value directly, ignoring shields. */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void AlterHealth(float Amount);

	/** Changes the health value directly, ignoring shields. */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void AlterShields(float Amount);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void StartShieldCooldown();

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void StartShieldRegen();

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void ShieldRegenRoutine();

	UPROPERTY(BlueprintReadWrite, Category = "HealthComponent")
	FTimerHandle ShieldsTimer;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnShieldsChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathSignature OnDeathEvent;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	static bool IsFriendly(AActor* actor1, AActor* actor2);
};
