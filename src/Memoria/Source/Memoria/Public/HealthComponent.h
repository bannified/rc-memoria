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
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Runtime")
	float currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Runtime")
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent|Gameplay")
	float startingHealth;

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
	void InitHealth(float health);

	/** [DEPRECATED] Subtracts `damage` from `currentHealth` */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void TakeDamage(float damage);

	/** Changes the health by parameter */
	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void AlterHealth(float Amount);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeathSignature OnDeathEvent;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	static bool IsFriendly(AActor* actor1, AActor* actor2);
};
