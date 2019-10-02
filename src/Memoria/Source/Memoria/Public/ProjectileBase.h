// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MemoriaTypes.h"
#include "ProjectileBase.generated.h"

struct FHitResult;

class UProjectileMovementComponent;
class UCapsuleComponent;
class UStaticMeshComponent;
class UParticleSystem;
class UCharacterProjectileEffect;
class UEnemyProjectileEffect;
class ACharacterBase;
class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileResolveDelegate, AProjectileBase*, projectile, const FHitResult&, Hit);

UCLASS()
class MEMORIA_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	AActor* OwningActor;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	AController* OwningController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	uint8 TeamNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	TArray< UCharacterProjectileEffect* > CharacterEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	TArray< UEnemyProjectileEffect* > UnitEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	float DamageDealt;

	// maybe move to another projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	float AoeRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	float Lifetime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	bool CanFriendlyFire;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileBase|Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProjectileBase|Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CollisionComp;	

	FTimerHandle lifetime_timerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Effects")
	UParticleSystem* TrailEmitter;

	// Projectile triggers upon hitting a
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileBase|Gameplay")
	ETargettingRule TriggerRule;// = ETargettingRule::EnemiesOnly;

	void DestroySelf();

	UFUNCTION()
	void OnHitComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ProjectileBase|Gameplay")
	void ClearIgnoredActors();

	UFUNCTION(BlueprintCallable, Category = "ProjectileBase|Gameplay")
	void AddIgnoredActor(AActor* actor);

private:

	void ResolveAllEffects(UHealthComponent* healthComp, ACharacterBase* character, ACharacterBase* enemy, const FHitResult& Hit);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnProjectileResolveDelegate OnProjectileResolve;

};
