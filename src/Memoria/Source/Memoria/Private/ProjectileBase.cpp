// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyProjectileEffect.h"
#include "CharacterProjectileEffect.h"
#include "HealthComponent.h"
#include "CharacterBase.h"
#include "DrawDebugHelpers.h"
#include "Memoria.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AIModule/Classes/Perception/AISense_Damage.h"
#include "AIModule/Classes/Perception/AISense_Hearing.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "MemoriaDamageType.h"
#include "GameFramework/DamageType.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CollisionComp->InitCapsuleSize(5.0f, 5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	CollisionComp->bTraceComplexOnMove = true;
	CollisionComp->bReturnMaterialOnMove = true;
	CollisionComp->SetGenerateOverlapEvents(true);

	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionResponseToChannels(ECR_Ignore);

	MeshComp->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	SetReplicates(true);
	SetReplicateMovement(true);

	Lifetime = 3.0f;
	AoeRadius = 0.0f;
	DamageDealt = 5.0f;

	TriggerRule = ETargettingRule::EnemiesOnly;
}

void AProjectileBase::DestroySelf()
{
	Destroy();
}

void AProjectileBase::OnHitComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UHealthComponent* actorHealthComp = nullptr;
	ACharacterBase* characterCast = nullptr;
	ACharacterBase* enemyCast = nullptr;
	bool isFriendly = false;

	if (AoeRadius <= 0.0f) {
		if (OtherActor == nullptr) {
			DestroySelf();
			return;
		}

		UActorComponent* comp = OtherActor->GetComponentByClass(UHealthComponent::StaticClass());

		if (comp == nullptr) {
			// do bouncing logic here?
			DestroySelf();
			return;
		}

		actorHealthComp = Cast<UHealthComponent>(comp);
		if (actorHealthComp == nullptr) {
			return;
		}

	}

	if (OtherActor != nullptr) {
		characterCast = Cast<ACharacterBase>(OwningActor);
		enemyCast = Cast<ACharacterBase>(OtherActor);

		if (actorHealthComp != nullptr) {
			isFriendly = TeamNumber == actorHealthComp->TeamNumber;
		}
	}

	switch (TriggerRule) {
		case ETargettingRule::EnemiesOnly:
			if (!isFriendly) {
				ResolveAllEffects(actorHealthComp, characterCast, enemyCast, Hit);
			}
			break;
		case ETargettingRule::FriendlyOnly:
			if (isFriendly) {
				ResolveAllEffects(actorHealthComp, characterCast, enemyCast, Hit);
			}
			break;
		case ETargettingRule::All:
			ResolveAllEffects(actorHealthComp, characterCast, enemyCast, Hit);
			break;
	}

	// We assume that we're consuming the projectile everytime we hit a target with a HealthComponent.
	DestroySelf();
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(lifetime_timerHandle, this, &AProjectileBase::DestroySelf, Lifetime, false);

	UGameplayStatics::SpawnEmitterAttached(TrailEmitter,
										   RootComponent,
										   NAME_None,
										   FVector::ZeroVector,
										   FRotator::ZeroRotator,
										   FVector(0.3f, 0.3f, 0.3f),
										   EAttachLocation::KeepRelativeOffset,
										   true);
	
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHitComponent);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::ClearIgnoredActors()
{
	CollisionComp->ClearMoveIgnoreActors();
}

void AProjectileBase::AddIgnoredActor(AActor* actor)
{
	CollisionComp->IgnoreActorWhenMoving(actor, true);
}

void AProjectileBase::ResolveAllEffects(UHealthComponent* healthComp, ACharacterBase* characterBase, ACharacterBase* enemy, const FHitResult& Hit)
{
	float finalDamage = DamageDealt;
	TSubclassOf<UMemoriaDamageType> finalDamageType = NormalDamageType;
	EPhysicalSurface surfaceType = SurfaceType_Default;
	surfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

	if (surfaceType == SURFACE_CRITICAL) {
		DamageDealt *= CriticalMultiplier.GetValue();
		finalDamageType = CriticalDamageType;
	}

	if (AoeRadius > 0.0f) {
		UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageDealt, Hit.ImpactPoint, AoeRadius, finalDamageType, TArray<AActor*>(), OwningActor, OwningController, true, COLLISION_PROJECTILEAOEBLOCK);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), Hit.ImpactPoint, 1.0F, OwningActor, AoeRadius);
		characterBase->OnDealDamage.Broadcast(Hit.ImpactPoint, finalDamageType->GetDefaultObject<UDamageType>(), characterBase, enemy);
	}
	else {
		UGameplayStatics::ApplyDamage(Hit.GetActor(), DamageDealt, OwningController, OwningActor, finalDamageType);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), Hit.ImpactPoint, 1.f, OwningActor);
		characterBase->OnDealDamage.Broadcast(Hit.ImpactPoint, finalDamageType->GetDefaultObject<UDamageType>(), characterBase, enemy);
	}

	if (enemy != nullptr) {
		for (UEnemyProjectileEffect* eff : UnitEffects) {
			eff->OnProjectileHit(this, enemy, Hit);
		}
	}

	if (characterBase != nullptr) {
		for (UCharacterProjectileEffect* eff : CharacterEffects) {
			eff->OnProjectileHit(this, characterBase, Hit);
		}
	}

	OnProjectileResolve.Broadcast(this, Hit);
}

