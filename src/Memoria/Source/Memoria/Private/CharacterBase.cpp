// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "UserWidget.h"
#include "HealthComponent.h"
#include "UnrealNetwork.h"
#include "CharacterPerkComponent.h"
#include "Components/ActorComponent.h"
#include "CharacterAttack.h"
#include "Memoria.h"
#include "GameTeam.h"
#include "MultiJumpPerkComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ManaComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));

	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bDoCollisionTest = false;

	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetAutoActivate(false);
	SpringArmComponent->Activate(false);

	MainCamera->SetupAttachment(SpringArmComponent);
	MainCamera->SetRelativeLocation(FVector(0.0, 50.0, 80.0));
	MainCamera->SetAutoActivate(false);
	MainCamera->Activate(false);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	GameplayCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GameplayCollisionComponent"));
	GameplayCollisionComponent->SetCanEverAffectNavigation(false);
	GameplayCollisionComponent->SetCollisionObjectType(ECC_WorldDynamic);
	GameplayCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GameplayCollisionComponent->SetCollisionProfileName(FName("GameplayOverlaps"));

	GameplayCollisionComponent->SetupAttachment(RootComponent);

	WidgetRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetRootComponent"));
	WidgetRootComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	ManaComponent = CreateDefaultSubobject<UManaComponent>(TEXT("ManaComponent"));

	GameplayWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ManaWidget"));
	GameplayWidget->SetWidgetSpace(EWidgetSpace::Screen);
	GameplayWidget->SetupAttachment(WidgetRootComponent);

	BoosterEndSocketName = "booster_end";
	NozzleEndSocketName = "nozzle_end";
	ReloadSocketName = "reload";

	Boost_Force = 3000.0f;
	Boost_Air_Force = 3000.0f;
	Boost_Cooldown = FModifiableAttribute(1.2f);
	b_CanBoost = true;

	GetCharacterMovement()->BrakingFriction = 1.0f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	CurrentMatter = 100.0f;

	WeaponToLevelMap = {};

	WeaponSocketName = FName("weapon_hold");

	GameplayScoreValue = 2.0f;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	/**
	 * Gameplay Stats defaults
	 */
	StatCooldownReduction = FModifiableAttribute(0.0f);
	StatAbilityDamage = FModifiableAttribute(10.0f);

	StatBaseDamage = FModifiableAttribute(5.0f);
	StatDamageMultiplier = FModifiableAttribute(1.0f);
	StatBaseAttackSpeed = FModifiableAttribute(1.0f);

	StatBaseKnockback = FModifiableAttribute(100000.0f);

	StatMovementSpeed = FModifiableAttribute(1000.0f);
	StatJumpVelocity = FModifiableAttribute(1200.0f);
	StatGravityScale = FModifiableAttribute(2.0f);

	/**
	 * AI Blackboard Defaults
	 */
	MinEngagementRange = FModifiableAttribute(300.0f);
	MaxEngagementRange = FModifiableAttribute(1500.0f);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ACharacterBase::ModifyCurrentMatter(float delta)
{
	CurrentMatter += delta;
}

void ACharacterBase::PossessedByPlayerControllerBase(APlayerControllerBase* controllerBase)
{
	SpringArmComponent->Activate(true);
	SpringArmComponent->bDoCollisionTest = true;
	MainCamera->Activate(true);

	if (CrosshairWidgetClass != nullptr) {
		// Spawn Crosshair Widget
		CrosshairWidget = CreateWidget<UUserWidget, APlayerControllerBase>(controllerBase, CrosshairWidgetClass, FName("Crosshair"));

		if (CrosshairWidget != nullptr) {
			CrosshairWidget->AddToPlayerScreen(200);
		}
		// do setup in BP.
	}

	OnReceivePossessedByPlayerControllerBase(controllerBase);
}

void ACharacterBase::UnPossessedByPlayerControllerBase(APlayerControllerBase* controllerBase)
{
	SpringArmComponent->Activate(false);
	SpringArmComponent->bDoCollisionTest = false;
	MainCamera->Activate(false);

	if (CrosshairWidget != nullptr) {
		CrosshairWidget->RemoveFromViewport();
		CrosshairWidget = nullptr;
	}

	OnReceiveUnPossessedByPlayerControllerBase(controllerBase);
}

void ACharacterBase::UpdateMovementProperties()
{
	GetCharacterMovement()->MaxWalkSpeed = StatMovementSpeed.GetValue();
	GetCharacterMovement()->JumpZVelocity = StatJumpVelocity.GetValue();
	GetCharacterMovement()->GravityScale = StatGravityScale.GetValue();
}

void ACharacterBase::DestroySelf_Implementation()
{
	for (int i = Attacks.Num() - 1; i >= 0; i--) {
		ACharacterAttack* instance = Attacks[i];
		if (instance == nullptr) {
			continue;
		}
		instance->TeardownWithCharacter(this);
		instance->Destroy();
	}
	Attacks.Empty();

	for (int i = CharacterPerks.Num() - 1; i >= 0; i--) {
		if (CharacterPerks[i] == nullptr) {
			continue;
		}
		CharacterPerks[i]->Teardown(this);
	}

	CharacterPerks.Empty();

	OnDestroy.Broadcast(this);
	Destroy();
}

bool ACharacterBase::DestroySelf_Validate()
{
	return true;
}

void ACharacterBase::EquipWeapon_Implementation(TSubclassOf<AWeaponBase> WeaponClass)
{
	//spawn the default weapon
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// this variable is only set on the server
	EquippedWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);

	if (EquippedWeapon) {
		EquippedWeapon->SetupWithCharacter(this);
		EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	}
}

bool ACharacterBase::EquipWeapon_Validate(TSubclassOf<AWeaponBase> Weapon)
{
	return true;
}

void ACharacterBase::AddLevelsToWeapon_Implementation(UWeaponDataAsset* weapon, int levels)
{
	if (!WeaponToLevelMap.Contains(weapon)) {
		WeaponToLevelMap.FindOrAdd(weapon);
		WeaponToLevelMap[weapon] += levels - 1;
	}
	else {
		WeaponToLevelMap[weapon] += levels;
	}
}

bool ACharacterBase::AddLevelsToWeapon_Validate(UWeaponDataAsset* weapon, int levels)
{
	return true;
}

void ACharacterBase::UpgradeWeapon_Implementation(UWeaponDataAsset* weapon)
{
	AddLevelsToWeapon(weapon, 1);
}

bool ACharacterBase::UpgradeWeapon_Validate(UWeaponDataAsset* weapon)
{
	return weapon->IsBuyableByCharacter(this);
}


void ACharacterBase::SetWeapon_Implementation(UWeaponDataAsset* weapon)
{

}

bool ACharacterBase::SetWeapon_Validate(UWeaponDataAsset* weapon)
{
	return true;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (DefaultWeaponClass != nullptr) {
		EquipWeapon(DefaultWeaponClass);
	}

	//if (HealthComponent != nullptr) {
	//	HealthComponent->DeathEvent.AddDynamic(this, &ACharacterBase::DestroySelf);
	//}

	// Setup Perk Components
	TArray< UActorComponent* > collectedActors = GetComponentsByClass(UCharacterPerkComponent::StaticClass());

	CharacterPerks = reinterpret_cast< TArray< UCharacterPerkComponent* >& >(collectedActors);

	Attacks.Reserve(AttacksClasses.Num());
	// Setup Action Components
	for (TSubclassOf<ACharacterAttack> attackClass : AttacksClasses) {
		ACharacterAttack* instance = GetWorld()->SpawnActor<ACharacterAttack>(attackClass, FVector::ZeroVector, FRotator::ZeroRotator);
		instance->AttackIndex = Attacks.Add(instance);
		instance->SetupWithCharacter(this);
	}

	for (int i = CharacterPerks.Num() - 1; i >= 0; i--) {
		CharacterPerks[i]->Setup(this);
	}

	HealthComponent->FullRestoreHealthComponent();

	UpdateMovementProperties();

	OnBeginPlayComplete();
}

UCharacterPerkComponent* ACharacterBase::AddPerk(TSubclassOf<UCharacterPerkComponent> perkClass)
{
	UCharacterPerkComponent* perk = NewObject<UCharacterPerkComponent>(this, perkClass);
	perk->RegisterComponent();
	CharacterPerks.Add(perk);
	//perk->Setup(this);

	return perk;
}

void ACharacterBase::RemoveAndTeardownPerk(UCharacterPerkComponent* perk)
{
	CharacterPerks.RemoveSingle(perk);
	perk->Teardown(this);
}

void ACharacterBase::MoveForward(float value)
{
	FRotator rot = FRotator(0, 0, 0);

	UKismetMathLibrary::BreakRotator(GetControlRotation(), rot.Roll, rot.Pitch, rot.Yaw);
	rot.Roll = 0;
	rot.Pitch = 0;

	FVector resultVector = UKismetMathLibrary::GetForwardVector(rot);

	AddMovementInput(resultVector, value);

	ReceiveMoveForward(value);
}

void ACharacterBase::MoveRight(float value)
{
	FRotator rot = FRotator(0, 0, 0);

	UKismetMathLibrary::BreakRotator(GetControlRotation(), rot.Roll, rot.Pitch, rot.Yaw);
	rot.Roll = 0;
	rot.Pitch = 0;

	FVector resultVector = UKismetMathLibrary::GetRightVector(rot);

	AddMovementInput(resultVector, value);

	ReceiveMoveRight(value);
}

void ACharacterBase::MoveUp_Implementation(float value)
{
	GetCharacterMovement()->AddForce(FVector(0.0, 0.0, value));
	ReceiveMoveUp(value);
}

bool ACharacterBase::MoveUp_Validate(float value)
{
	return true;
}

bool ACharacterBase::AttackStart(int index)
{
	if (Attacks.Num() < index + 1) {
		return false;
	}

	if (Attacks[index] == nullptr) {
		return false;
	}

	if (CurrentActiveAttack != nullptr) 
	{
		if (!CurrentActiveAttack->IsInterruptable) 
		{
			return false; // Do not interrupt the ongoing attack
		}
	}

	CurrentActiveAttack = Attacks[index];

	Attacks[index]->AttackStart();
	return true;
}

bool ACharacterBase::AttackEnd(int index)
{
	if (Attacks.Num() < index + 1) {
		return false;
	}

	if (Attacks[index] == nullptr) {
		return false;
	}

	Attacks[index]->AttackEnd();

	if (CurrentActiveAttack == Attacks[index]) {
		CurrentActiveAttack = nullptr;
		return false;
	}

	return true;
}

void ACharacterBase::PrimaryFireStart()
{
	OnPrimaryStart.Broadcast(this);

	AttackStart(0);

	ReceivePrimaryFireStart();
}

void ACharacterBase::PrimaryFireEnd()
{
	OnPrimaryEnd.Broadcast(this);

	AttackEnd(0);

	ReceivePrimaryFireEnd();
}

void ACharacterBase::SecondaryFireStart()
{
	OnSecondaryStart.Broadcast(this);

	AttackStart(1);

	ReceiveSecondaryFireStart();
}

void ACharacterBase::SecondaryFireEnd()
{
	OnSecondaryEnd.Broadcast(this);

	AttackEnd(1);

	ReceiveSecondaryFireEnd();
}

void ACharacterBase::Special1Start()
{
	AttackStart(2);

	ReceiveSpecial1Start();
}

void ACharacterBase::Special1End()
{
	AttackEnd(2);

	ReceiveSpecial1End();
}

void ACharacterBase::Special2Start()
{
	AttackStart(3);

	ReceiveSpecial2Start();
}

void ACharacterBase::Special2End()
{
	AttackEnd(3);

	ReceiveSpecial2End();
}

void ACharacterBase::ReloadStart()
{
	if (ManaComponent->IsReloading()) {
		return;
	}

	ManaComponent->StartReload();

	if (ReloadingSound != nullptr) {
		USceneComponent* attachComponent = GetWeaponMeshComponent();
		UGameplayStatics::SpawnSoundAttached(ReloadingSound, attachComponent, ReloadSocketName, FVector::ZeroVector, EAttachLocation::SnapToTargetIncludingScale, true);	
	}
	ReceiveReloadStart();
}

void ACharacterBase::ReloadEnd()
{
	ReceiveReloadEnd();
}

void ACharacterBase::PreviousEquipment()
{
	ReceivePreviousEquipment();
}

void ACharacterBase::NextEquipment()
{
	ReceiveNextEquipment();
}

void ACharacterBase::MovementModStart()
{
	BoostAction();

	ReceiveMovementModStart();
}

void ACharacterBase::MovementModEnd()
{
	ReceiveMovementModEnd();
}

void ACharacterBase::JumpStart()
{
	OnJumpStart.Broadcast(this, JumpCurrentCount);
	Jump();
	ReceiveJumpStart();
}

void ACharacterBase::JumpEnd()
{
	OnJumpEnd.Broadcast(this, JumpCurrentCount);
	ReceiveJumpEnd();
}

void ACharacterBase::InteractStart()
{
	ReceiveInteractStart();
}

void ACharacterBase::InteractEnd()
{
	ReceiveInteractEnd();
}

void ACharacterBase::Escape()
{
	ReceiveEscape();
}

void ACharacterBase::Confirm()
{
	ReceiveConfirm();
}

void ACharacterBase::Pause()
{
	ReceivePause();
}

void ACharacterBase::Contextual()
{
	ReceiveContextual();
}

void ACharacterBase::ReInitializeAttacks(TArray<TSubclassOf<ACharacterAttack>> attackClasses)
{
	for (int i = Attacks.Num() - 1; i >= 0; i--) {
		Attacks[i]->TeardownWithCharacter(this);
		Attacks.RemoveAt(i);
	}

	AttacksClasses = attackClasses;

	Attacks.Reserve(AttacksClasses.Num());
	// Setup Action Components
	for (TSubclassOf<ACharacterAttack> attackClass : AttacksClasses) {
		ACharacterAttack* instance = GetWorld()->SpawnActor<ACharacterAttack>(attackClass, FVector::ZeroVector, FRotator::ZeroRotator);
		instance->AttackIndex = Attacks.Add(instance);
		instance->SetupWithCharacter(this);
	}
}

void ACharacterBase::PlayBoostEffects_Implementation()
{

}

void ACharacterBase::BoostAction_Implementation()
{
	if (!b_CanBoost) {
		return;
	}

	FVector inputDirection = GetPlayerControllerBase()->GetInputAxisValue(FName("MoveForward")) * MainCamera->GetForwardVector()
		+ GetPlayerControllerBase()->GetInputAxisValue(FName("MoveRight")) * MainCamera->GetRightVector();

	inputDirection.Z = 0.0f;

	// Resets lateral velocity. Maintains vertical velocity.
	GetCharacterMovement()->Velocity.X = 0.0f;
	GetCharacterMovement()->Velocity.Y = 0.0f;
	GetCharacterMovement()->UpdateComponentVelocity();

	if (GetCharacterMovement()->IsMovingOnGround()) {
		AddImpulseToCharacterInDirectionWithMagnitude(inputDirection, Boost_Force.GetValue());
	}
	else {
		AddImpulseToCharacterInDirectionWithMagnitude(inputDirection, Boost_Air_Force.GetValue());
	}

	this->b_CanBoost = false;

	OnBoostActivated.Broadcast(this);

	PlayBoostEffects();

	GetWorldTimerManager().SetTimer(Boost_Cooldown_TimerHandle, [&]()
	{
		this->b_CanBoost = true;
		OnBoostOffCooldown.Broadcast(this);
	}, Boost_Cooldown.GetValue(), false);
}

bool ACharacterBase::BoostAction_Validate()
{
	return true;
}

void ACharacterBase::AddImpulseToCharacterInDirectionWithMagnitude_Implementation(const FVector directionalVector, const float impulseMagnutide)
{
	GetCharacterMovement()->AddImpulse(directionalVector * impulseMagnutide, true);
}

bool ACharacterBase::AddImpulseToCharacterInDirectionWithMagnitude_Validate(const FVector directionalVector, const float impulseMagnutide)
{
	return true;
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	HealthComponent->TeamNumber = (uint8)(EGameTeam)TeamID.GetId();
}

FGenericTeamId ACharacterBase::GetGenericTeamId() const
{
	return uint8(HealthComponent->TeamNumber);
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterBase, b_CanBoost);
	DOREPLIFETIME(ACharacterBase, WeaponToLevelMap);

	DOREPLIFETIME(ACharacterBase, CurrentMatter);
	DOREPLIFETIME(ACharacterBase, EquippedWeapon);
}