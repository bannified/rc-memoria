// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "NozzleBase.generated.h"

class ACharacterBase;

class UWeaponDataAsset;
class UParticleSystem;

UCLASS()
class MEMORIA_API ANozzleBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANozzleBase();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool IsActorWithinRange(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool IsPathObstructed(AActor* TargetActor);

	UFUNCTION(NetMulticast, Reliable, WithValidation, BlueprintCallable, Category = "Gameplay")
	void SetupWithWeaponData(UWeaponDataAsset* weaponData);

protected:

	/* Components */

	/* Runtime Gameplay variables */
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int CurrentLevel;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	UWeaponDataAsset* NozzleData;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	ACharacterBase* Character;

	/* Settings */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	TArray< TEnumAsByte<EObjectTypeQuery> > ObstacleTypes;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<UParticleSystem> SecondaryParticleSystem;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	FVector SecondaryParticleScale;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	FName SecondaryParticleSystemSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float VelocityLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float DeadZoneLimit;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
