// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UHealthComponent;

UCLASS()
class MEMORIA_API ABase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	UHealthComponent* HealthComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MainMesh;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* DamageTriggerMesh;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
	USphereComponent* DamageTriggerCollider;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float StartingHealth;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Hide();
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void Reveal();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	void OnHide();
	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	void OnReveal();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
