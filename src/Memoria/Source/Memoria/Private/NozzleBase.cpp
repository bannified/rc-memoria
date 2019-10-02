// Fill out your copyright notice in the Description page of Project Settings.


#include "NozzleBase.h"
#include "UnrealNetwork.h"

// Sets default values
ANozzleBase::ANozzleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentLevel = 1;
	ObstacleTypes = { EObjectTypeQuery::ObjectTypeQuery1 };

	SecondaryParticleScale = FVector(1.0f, 1.0f, 1.0f);
	SecondaryParticleSystemSocketName = "ParticleSystemOut";

	VelocityLimit = 200.0f;
	DeadZoneLimit = 200.0f;
}

bool ANozzleBase::IsActorWithinRange(AActor* TargetActor)
{
	return true;
}

bool ANozzleBase::IsPathObstructed(AActor* TargetActor)
{
	return true;
}

void ANozzleBase::SetupWithWeaponData_Implementation(UWeaponDataAsset* weaponData)
{

}

bool ANozzleBase::SetupWithWeaponData_Validate(UWeaponDataAsset* weaponData)
{
	return true;
}

// Called when the game starts or when spawned
void ANozzleBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ANozzleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANozzleBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANozzleBase, CurrentLevel);
	DOREPLIFETIME(ANozzleBase, NozzleData);
	DOREPLIFETIME(ANozzleBase, Character);

}

