// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USoundCue;
class USceneComponent;
class USkeletalMeshComponent;
class USkeletalMesh;
class UAudioComponent;
class ANozzleBase;
class ACharacterBase;
class UWeaponDataAsset;
class AProjectileBase;
class UParticleSystem;

UCLASS()
class MEMORIA_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void SetupWithCharacter(ACharacterBase* character);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void PrimaryStart();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void PrimaryUpdate();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void PrimaryEnd();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void PrimaryFire();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void SecondaryStart();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void SecondaryUpdate();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void SecondaryEnd();
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void SecondaryFire();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void SetNozzleProperties(UWeaponDataAsset* weaponData, int level);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Weapons")
	void InitNozzle(ANozzleBase* Nozzle, UWeaponDataAsset* nozzleData);

	UFUNCTION(BlueprintCallable, Category = "Utility")
	float ConvertRawMatterToFinalMatter(float inMatter);

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "WeaponBase|Audio")
	void PlayPrimarySound();
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "WeaponBase|Audio")
	void PlaySecondarySound();
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "WeaponBase|Audio")
	void StopPrimarySound();

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Settings */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<USkeletalMesh> WeaponMeshClassOverride;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	ACharacterBase* Character;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	ANozzleBase* CurrentNozzle;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	UWeaponDataAsset* CurrentWeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* BlowSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* SuckSoundCue;

	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = "Weapon")
	void PlayPrimaryFireEffects();

	/* Runtime variables */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "WeaponBase|Gameplay")
	float PrimaryBaseCooldown;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "WeaponBase|Gameplay")
	float SecondaryBaseCooldown;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "WeaponBase|Gameplay")
	float BulletSpreadAngle;

	FTimerHandle primaryFire_timerHandle;
	FTimerHandle secondaryFire_timerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "_Gameplay")
	FName MuzzleSocketName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "WeaponBase|Effects")
	TSubclassOf<AProjectileBase> PrimaryProjectileClass;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "WeaponBase|Effects")
	TSubclassOf<AProjectileBase> SecondaryProjectileClass;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "WeaponBase|Effects")
	UParticleSystem* PrimaryMuzzleEffect;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "WeaponBase|Effects")
	UParticleSystem* SecondaryMuzzleEffect;

	/* Virtual getters */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual float GetPrimaryCooldown();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	virtual float GetSecondaryCooldown();

	float LastPrimaryFireTime = -1.0f;
	float LastSecondaryFireTime = -1.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
