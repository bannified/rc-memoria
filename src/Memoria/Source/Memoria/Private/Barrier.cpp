// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrier.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CharacterBase.h"
#include "HealthComponent.h"
#include "MemoriaStaticLibrary.h"

// Sets default values
ABarrier::ABarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));

	SetRootComponent(SceneRootComponent);

	CollisionMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMeshComponent"));
	CollisionMeshComponent->SetupAttachment(SceneRootComponent);

	VisualMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMeshComponent"));
	VisualMeshComponent->SetupAttachment(SceneRootComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

}

void ABarrier::Deactivate()
{
	UMemoriaStaticLibrary::SetActorEnabled(this, false);
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->DeathEvent.AddDynamic(this, &ABarrier::Deactivate);
}

// Called every frame
void ABarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

