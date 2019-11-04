// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModifiableAttribute.h"
#include "ManaComponent.generated.h"

class ACharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterReload, ACharacterBase*, character, UManaComponent*, manaComp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FManaChangeDelegate, ACharacterBase*, character, UManaComponent*, manaComp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEMORIA_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UManaComponent();

	ACharacterBase* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManaComponent")
	FModifiableAttribute MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManaComponent")
	float CurrentMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManaComponent")
	float StartingMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManaComponent")
	float ReloadValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ManaComponent")
	FModifiableAttribute ReloadTime;

	UPROPERTY(BlueprintAssignable, Category = "ManaComponent")
	FCharacterReload ReloadStartEvent;

	UPROPERTY(BlueprintAssignable, Category = "ManaComponent")
	FCharacterReload ReloadInterruptEvent;

	UPROPERTY(BlueprintAssignable, Category = "ManaComponent")
	FCharacterReload ReloadCompleteEvent;

	UPROPERTY(BlueprintAssignable, Category = "ManaComponent")
	FManaChangeDelegate OnManaChangeEvent;

	UFUNCTION(BlueprintCallable, Category = "ManaComponent")
	void ModifyMana(float value, bool overflow = false);

	FTimerHandle ReloadTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "ManaComponent")
	void StartReload();

	UFUNCTION(BlueprintCallable, Category = "ManaComponent")
	void InterruptReload();

	UFUNCTION(BlueprintCallable, Category = "ManaComponent")
	void ReloadDone();

	// Returns -1.f if timer does not exist
	UFUNCTION(BlueprintCallable, Category = "ManaComponent")
	FORCEINLINE float GetTimeRemainingForReload() { return GetWorld()->GetTimerManager().GetTimerRemaining(ReloadTimerHandle); };
	UFUNCTION(BlueprintCallable, Category = "ManaComponent")
	FORCEINLINE float GetTimeElapsedForReload() { return GetWorld()->GetTimerManager().GetTimerElapsed(ReloadTimerHandle); };

	UFUNCTION(BlueprintCallable, Category = "ManaComponent")
	FORCEINLINE bool IsReloading() { return GetWorld()->GetTimerManager().TimerExists(ReloadTimerHandle); }

	UFUNCTION(BlueprintCallable, Category = "ManaComponent")
	void FullRestoreMana();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
