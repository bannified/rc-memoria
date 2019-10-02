// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barrier.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UHealthComponent;
class ACharacterBase;

UCLASS()
class MEMORIA_API ABarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarrier();

	/* Components */

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Barrier")
	USceneComponent* SceneRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Barrier")
	UStaticMeshComponent* CollisionMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Barrier")
	UStaticMeshComponent* VisualMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Barrier")
	UHealthComponent* HealthComponent;

	UFUNCTION(BlueprintCallable, Category = "Barrier")
	void Deactivate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
