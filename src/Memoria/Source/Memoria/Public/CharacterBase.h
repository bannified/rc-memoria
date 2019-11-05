// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerControllerBase.h"
#include "WeaponDataAsset.h"
#include "HealthComponent.h"
#include "WeaponBase.h"
#include "GenericTeamAgentInterface.h"
#include "ModifiableAttribute.h"
#include "CharacterBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AWeaponBase;
class UHealthComponent;
class UBehaviorTree;
class USpawnUnitAsset;
class UWidgetComponent;
class UUserWidget;
class UCharacterPerkComponent;
class ACharacterAttack;
class UManaComponent;
class USceneComponent;
class UAnimMontage;
class UWorld;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseGameplayEvent, ACharacterBase*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterBaseGameplayEvent_OneInt, ACharacterBase*, Character, int, value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDamageDealt, FVector, ImpactPoint, const class UDamageType*, DamageType, AActor*, DamageCauser, AActor*, Victim);

UCLASS()
class MEMORIA_API ACharacterBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	friend APlayerControllerBase;

public:
	// Sets default values for this character's properties
	ACharacterBase();

	/** Returns controller for this actor. */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE APlayerControllerBase* GetPlayerControllerBase()
	{
		return Cast<APlayerControllerBase>(GetController());
	};

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool HasWeapon(UWeaponDataAsset* weapon)
	{
		return WeaponToLevelMap.Contains(weapon);
	}

	// Returns nullptr if weapon does not exist.
	FORCEINLINE const int* GetWeaponCurrentLevel(const UWeaponDataAsset* weapon)
	{
		return WeaponToLevelMap.Find(weapon);
	}

	FORCEINLINE float GetCurrentMatter() { return CurrentMatter; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetTeamNumber() { return HealthComponent->TeamNumber; }

	UFUNCTION(BlueprintCallable)
	void ModifyCurrentMatter(float delta);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Equipment")
	void UpgradeWeapon(UWeaponDataAsset* weapon);
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Equipment")
	void AddLevelsToWeapon(UWeaponDataAsset* weapon, int levels);

	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = "Equipment")
	void SetWeapon(UWeaponDataAsset* weapon);

	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = "Equipment")
	void EquipWeapon(TSubclassOf<AWeaponBase> Weapon);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Equipment")
	FORCEINLINE USkeletalMeshComponent* GetWeaponMeshComponent() { return 
		(EquippedWeapon != nullptr) ? EquippedWeapon->WeaponMeshComponent : GetMesh(); 
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	USpawnUnitAsset* SpawnUnitAsset;

	virtual void PossessedByPlayerControllerBase(APlayerControllerBase* controllerBase);
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase Events")
	void OnReceivePossessedByPlayerControllerBase(APlayerControllerBase* controllerBase);

	virtual void UnPossessedByPlayerControllerBase(APlayerControllerBase* controllerBase);
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase Events")
	void OnReceiveUnPossessedByPlayerControllerBase(APlayerControllerBase* controllerBase);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "CharacterBase")
	virtual void DestroySelf();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase Events")
	TArray<AActor*> ActorsToIgnoreWhileAttacking;

	/* Gameplay Events */

	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent OnBoostActivated;
	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent OnBoostOffCooldown;

	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent OnDestroy;

	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent OnPrimaryStart;
	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent OnPrimaryEnd;

	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent OnSecondaryStart;
	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent OnSecondaryEnd;

	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent_OneInt OnPrimaryShot;
	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent_OneInt OnSecondaryShot;
	
	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent_OneInt OnJumpStart;
	UPROPERTY(BlueprintAssignable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent_OneInt OnJumpEnd;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "CharacterBase Events")
	FOnDamageDealt OnDealDamage;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent OnAttackTrigger_AnimNotify;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "CharacterBase Events")
	FCharacterBaseGameplayEvent OnAttackEnd_AnimNotify;

	/* End of Gameplay Events*/

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Equipment")
	AWeaponBase* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* MainCamera;

    /* Gameplay Properties */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
	FModifiableAttribute StatCooldownReduction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
	FModifiableAttribute StatAbilityDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
	FModifiableAttribute StatBaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
	FModifiableAttribute StatDamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
	FModifiableAttribute StatBaseAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
	FModifiableAttribute StatBaseKnockback;

    /* General Gameplay Value used during GameModeStates, has no meaning in and of itself */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
    FModifiableAttribute GameplayScoreValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
	FModifiableAttribute StatMovementSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
	FModifiableAttribute StatJumpVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Gameplay")
	FModifiableAttribute StatGravityScale;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Gameplay")
	void UpdateMovementProperties();

protected:

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Equipment")
	TMap<UWeaponDataAsset*, int> WeaponToLevelMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
	float CameraSensitivity;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float CurrentMatter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment")
	TSubclassOf<AWeaponBase> DefaultWeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipment")
	FName WeaponSocketName;

	/**
	 * Gameplay components
	 */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase")
	void OnBeginPlayComplete();
public:
	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UHealthComponent* HealthComponent;

	UPROPERTY(Replicated, VisibleDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UManaComponent* ManaComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Components")
	UCapsuleComponent* GameplayCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* UnitBehaviorTreeAsset;

	/**
	 * Widgets / Widget Components
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USceneComponent* WidgetRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* GameplayWidget;

	// Crosshair, only spawned when possessed by a PlayerControllerBase
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UUserWidget* CrosshairWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	/**
	 * Runtime variables
	 */

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "CharacterBase")
	TArray< UCharacterPerkComponent* > CharacterPerks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterBase")
	TArray< TSubclassOf<ACharacterAttack> > AttacksClasses;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "CharacterBase")
	TArray< ACharacterAttack* > Attacks;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	ACharacterAttack* CurrentActiveAttack;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	FORCEINLINE ACharacterAttack* GetPrimaryAttack()
	{
		return (Attacks.Num() > 0) ? Attacks[0] : nullptr;
	};

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	FORCEINLINE ACharacterAttack* GetSecondaryAttack()
	{
		return (Attacks.Num() > 1) ? Attacks[1] : nullptr;
	};
	
	/**
	 * Character Animation montages
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|Animation")
	TMap< FName, UAnimMontage* > NameToAnimMap;

	// Returns nullptr if animation does not exist
	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Animation")
	FORCEINLINE UAnimMontage* GetAnimWithName(const FName name)
	{
		UAnimMontage** montage = NameToAnimMap.Find(name);
		if (montage == nullptr) return nullptr;
		return *(montage);
	}

	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Perks")
	UCharacterPerkComponent* AddPerk(TSubclassOf<UCharacterPerkComponent> perkClass);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase|Perks")
	void RemoveAndTeardownPerk(UCharacterPerkComponent* perkClass);

	/**
	 * Initialization
	 */

	/**
	 * AI Blackboard Initialization
	 */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|AI")
	FModifiableAttribute MinEngagementRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase|AI")
	FModifiableAttribute MaxEngagementRange;

	/**
	 * Movement Methods
	 */
protected:
	virtual void MoveForward(float value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void ReceiveMoveForward(float value);

	virtual void MoveRight(float value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void ReceiveMoveRight(float value);

	UFUNCTION(Server, Reliable, WithValidation)
	virtual void MoveUp(float value);
	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void ReceiveMoveUp(float value);

	/**
	 * Actions
	 */
	// Start an attack given an index. Returns true if there's a valid attack
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	bool AttackStart(int index);
	// Ends an attack given an index. Returns true if there's a valid attack
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	bool AttackEnd(int index);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void PrimaryFireStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceivePrimaryFireStart();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void PrimaryFireEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceivePrimaryFireEnd();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void SecondaryFireStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveSecondaryFireStart();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void SecondaryFireEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveSecondaryFireEnd();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void Special1Start();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveSpecial1Start();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void Special1End();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveSpecial1End();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void Special2Start();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveSpecial2Start();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void Special2End();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveSpecial2End();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void ReloadStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveReloadStart();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void ReloadEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveReloadEnd();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void PreviousEquipment();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceivePreviousEquipment();
	
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void NextEquipment();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveNextEquipment();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void MovementModStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveMovementModStart();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void MovementModEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveMovementModEnd();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void JumpStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveJumpStart();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void JumpEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void ReceiveJumpEnd();

	/**
	 * General
	 */
	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void InteractStart();
	UFUNCTION(BlueprintImplementableEvent, Category = "General")
	void ReceiveInteractStart();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void InteractEnd();
	UFUNCTION(BlueprintImplementableEvent, Category = "General")
	void ReceiveInteractEnd();

	virtual void Escape();
	UFUNCTION(BlueprintImplementableEvent, Category = "General")
	void ReceiveEscape();

	virtual void Confirm();
	UFUNCTION(BlueprintImplementableEvent, Category = "General")
	void ReceiveConfirm();

	virtual void Pause();
	UFUNCTION(BlueprintImplementableEvent, Category = "General")
	void ReceivePause();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	virtual void Contextual();
	UFUNCTION(BlueprintImplementableEvent, Category = "General")
	void ReceiveContextual();

	/**
	 * Thrusts player in a direction based on current velocity and input direction.
	 */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Advanced Movement")
	void BoostAction();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Advanced Movement")
	void PlayBoostEffects();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Movement")
	FModifiableAttribute Boost_Force;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Movement")
	FModifiableAttribute Boost_Air_Force;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Movement")
	FModifiableAttribute Boost_Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Advanced Movement")
	float b_CanBoost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Advanced Movement")
	FTimerHandle Boost_Cooldown_TimerHandle;

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "Advanced Movement")
	void AddImpulseToCharacterInDirectionWithMagnitude(const FVector directionalVector, const float impulseMagnutide);

	/**
	 * Visual/Sound Effects
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* BoosterSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	FVector BoosterParticleScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visuals")
	UParticleSystem* BoosterParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	FName BoosterEndSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Visuals")
	FName NozzleEndSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterAttack|ProjectileAttack")
	FName ReloadSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterBase|Audio")
	USoundBase* ReloadingSound;
	
	UFUNCTION(BlueprintCallable, Category = "CharacterAttack|Attacks")
	void ReInitializeAttacks(TArray<TSubclassOf<ACharacterAttack>> attackClasses);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
	FORCEINLINE UWorld* GetWorldContext()
	{
		return GetWorld();
	}

public:
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

};