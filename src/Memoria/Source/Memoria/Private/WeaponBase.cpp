// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "NozzleBase.h"
#include "WeaponDataAsset.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/AudioComponent.h"
#include "UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileBase.h"
#include "CharacterBase.h"
#include "Camera/PlayerCameraManager.h"
#include "Memoria.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(SceneRoot);

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMeshComponent->SetupAttachment(RootComponent);

	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	WeaponMeshComponent->SetWorldRotation(FRotator(0.0, 90.0f, 0.0f));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	BulletSpreadAngle = 2.0f;

	MuzzleSocketName = FName("MuzzleSocketName");
}

void AWeaponBase::SetupWithCharacter_Implementation(ACharacterBase* character)
{
	SetOwner(character);
	Character = character;
}

bool AWeaponBase::SetupWithCharacter_Validate(ACharacterBase* character)
{
	return true;
}

void AWeaponBase::PrimaryFire_Implementation()
{
	//PRINT_INFO("PrimaryFire called!");

	AActor* owner = GetOwner();

	if (owner) {
		FVector cameraLocation;
		FRotator cameraRotation;

		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraViewPoint(cameraLocation, cameraRotation);

		// bullet spread
		float halfRad = FMath::DegreesToRadians(BulletSpreadAngle);

		FActorSpawnParameters* spawnParams = new FActorSpawnParameters();
		spawnParams->Owner = owner;
		spawnParams->SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector muzzleLocation = WeaponMeshComponent->GetSocketLocation(MuzzleSocketName);

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(owner);
		queryParams.AddIgnoredActor(this);
		queryParams.bReturnPhysicalMaterial = true; // returns material that it hits (headshots, bodyshots, etc)
		queryParams.bTraceComplex = true;

		FHitResult result;

		FVector cameraViewVector = cameraLocation + cameraRotation.Vector() * 10000;

		FVector direction = cameraViewVector - muzzleLocation;

		if (GetWorld()->LineTraceSingleByChannel(result, cameraLocation, cameraViewVector, COLLISION_AIM_DETECT, queryParams))
		{
			direction = result.ImpactPoint - muzzleLocation;
		}

		direction = FMath::VRandCone(direction, halfRad, halfRad);

		direction.Normalize();

		AProjectileBase* projectile = GetWorld()->SpawnActor<AProjectileBase>(PrimaryProjectileClass, muzzleLocation, direction.Rotation(), *spawnParams);
		/* Setup projectile properties, such as Damage, TeamNumber, AOE etc */
		projectile->TeamNumber = Character->GetTeamNumber();
		projectile->SetOwner(owner);
		projectile->OwningController = Character->GetController();
		projectile->OwningActor = Character;
		projectile->ClearIgnoredActors();
		projectile->AddIgnoredActor(Character);
		projectile->AddIgnoredActor(this);
		// set projectile damage.

		PlayPrimaryFireEffects();

		LastPrimaryFireTime = GetWorld()->TimeSeconds;
	}
}

bool AWeaponBase::PrimaryFire_Validate()
{
	return true;
}

void AWeaponBase::StopPrimarySound_Implementation()
{
	AudioComponent->Stop();
}

void AWeaponBase::PlaySecondarySound_Implementation()
{
	AudioComponent->Sound = BlowSoundCue;
	AudioComponent->Play(0.0f);
}

void AWeaponBase::PlayPrimarySound_Implementation()
{
	AudioComponent->Sound = SuckSoundCue;
	AudioComponent->Play(0.0f);
}

void AWeaponBase::SecondaryFire_Implementation()
{
	//PRINT_INFO("SecondaryFire called!");
	LastSecondaryFireTime = GetWorld()->TimeSeconds;

}

bool AWeaponBase::SecondaryFire_Validate()
{
	return true;
}

float AWeaponBase::ConvertRawMatterToFinalMatter(float inMatter)
{
	return 0.0f;
}

void AWeaponBase::InitNozzle_Implementation(ANozzleBase* Nozzle, UWeaponDataAsset* nozzleData)
{

}

bool AWeaponBase::InitNozzle_Validate(ANozzleBase* Nozzle, UWeaponDataAsset* nozzleData)
{
	return true;
}

void AWeaponBase::SetNozzleProperties_Implementation(UWeaponDataAsset* weaponData, int level)
{
	CurrentWeaponData = weaponData;
	// set nozzle's mesh
}

bool AWeaponBase::SetNozzleProperties_Validate(UWeaponDataAsset* weaponData, int level)
{
	return true;
}

void AWeaponBase::SecondaryStart_Implementation()
{
	float firstDelay = FMath::Max(LastSecondaryFireTime + GetSecondaryCooldown() - GetWorld()->TimeSeconds, 0.00f);

	GetWorldTimerManager().SetTimer(secondaryFire_timerHandle, this, &AWeaponBase::SecondaryFire, GetSecondaryCooldown(), true, firstDelay);
}

bool AWeaponBase::SecondaryStart_Validate()
{
	return true;
}


void AWeaponBase::SecondaryEnd_Implementation()
{
	GetWorldTimerManager().ClearTimer(secondaryFire_timerHandle);
}

bool AWeaponBase::SecondaryEnd_Validate()
{
	return true;
}

void AWeaponBase::SecondaryUpdate_Implementation()
{

}

bool AWeaponBase::SecondaryUpdate_Validate()
{
	return true;
}

void AWeaponBase::PrimaryUpdate_Implementation()
{

}

bool AWeaponBase::PrimaryUpdate_Validate()
{
	return true;
}

void AWeaponBase::PrimaryStart_Implementation()
{
	float firstDelay = FMath::Max(LastPrimaryFireTime + GetPrimaryCooldown() - GetWorld()->TimeSeconds, 0.00f);

	GetWorldTimerManager().SetTimer(primaryFire_timerHandle, this, &AWeaponBase::PrimaryFire, GetPrimaryCooldown(), true, firstDelay);

	Character->bUseControllerRotationYaw = true;
}

bool AWeaponBase::PrimaryStart_Validate()
{
	return true;
}

void AWeaponBase::PrimaryEnd_Implementation()
{
	GetWorldTimerManager().ClearTimer(primaryFire_timerHandle);
	Character->bUseControllerRotationYaw = false;
}

bool AWeaponBase::PrimaryEnd_Validate()
{
	return true;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	LastPrimaryFireTime = -GetPrimaryCooldown();
	LastSecondaryFireTime = -GetSecondaryCooldown();
}

void AWeaponBase::PlayPrimaryFireEffects_Implementation()
{
	if (PrimaryMuzzleEffect) {
		UGameplayStatics::SpawnEmitterAttached(PrimaryMuzzleEffect, WeaponMeshComponent, MuzzleSocketName);
	}
}

bool AWeaponBase::PlayPrimaryFireEffects_Validate()
{
	return true;
}

float AWeaponBase::GetPrimaryCooldown()
{
	return PrimaryBaseCooldown;
}

float AWeaponBase::GetSecondaryCooldown()
{
	return SecondaryBaseCooldown;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeaponBase, CurrentNozzle);
	DOREPLIFETIME(AWeaponBase, CurrentWeaponData);
	DOREPLIFETIME(AWeaponBase, Character);
	DOREPLIFETIME(AWeaponBase, PrimaryBaseCooldown);
	DOREPLIFETIME(AWeaponBase, SecondaryBaseCooldown);

	DOREPLIFETIME(AWeaponBase, LastPrimaryFireTime);
	DOREPLIFETIME(AWeaponBase, LastSecondaryFireTime);

	DOREPLIFETIME(AWeaponBase, PrimaryProjectileClass);
	DOREPLIFETIME(AWeaponBase, SecondaryProjectileClass);
}

