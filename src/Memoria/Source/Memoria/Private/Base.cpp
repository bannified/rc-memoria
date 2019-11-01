// Fill out your copyright notice in the Description page of Project Settings.

#include "Base.h"
#include "HealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABase::ABase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	SetRootComponent(MainMesh);

	MainMesh->SetSimulatePhysics(false);

	MainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainMesh->SetCollisionResponseToAllChannels(ECR_Block);

	MainMesh->SetCanEverAffectNavigation(true);

	DamageTriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DamageTrigger_Mesh"));
	DamageTriggerMesh->SetupAttachment(MainMesh);
	DamageTriggerMesh->SetSimulatePhysics(false);
	DamageTriggerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageTriggerMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	DamageTriggerMesh->SetCanEverAffectNavigation(false);

	DamageTriggerCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DamageTrigger_Collider"));
	DamageTriggerCollider->SetupAttachment(DamageTriggerMesh);
	DamageTriggerCollider->SetSimulatePhysics(false);
	DamageTriggerCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageTriggerCollider->SetCollisionResponseToAllChannels(ECR_Overlap);

	DamageTriggerCollider->SetCanEverAffectNavigation(false);

	DamageTriggerCollider->Bounds.SphereRadius = 450.0f;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->Init();
}

void ABase::Hide()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	OnHide();
}

void ABase::Reveal()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	OnReveal();
}

// Called every frame
void ABase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

