// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Engine.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DamageResist.BaseValue = 0.0f;

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	InitHealth(startingHealth);

	//if (GetOwnerRole() == ROLE_Authority)
	//{
		AActor* owner = GetOwner();
		if (owner)
		{
			owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
		}
	//}
}


void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead)
		return;

	//// avoid friendly fire
	//if (DamageCauser == DamagedActor) //&& IsFriendly(DamagedActor, DamageCauser))
	//{
	//	return;
	//}

	Damage *= FMath::Max(1.0f - DamageResist.GetValue(), 0.0f);

	currentHealth = FMath::Clamp(currentHealth - Damage, 0.0f, maxHealth);

	OnHealthChanged.Broadcast(this, currentHealth, Damage, DamageType, InstigatedBy, DamageCauser);

	bIsDead = currentHealth <= 0.0f;

	if (bIsDead)
	{
		OnDeathEvent.Broadcast(DamagedActor, DamageCauser);
		DeathEvent.Broadcast();
	}
}

float UHealthComponent::GetHealth() const
{
	return currentHealth;
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::InitHealth(float health)
{
	currentHealth = startingHealth;
	bIsDead = false;
}

void UHealthComponent::TakeDamage(float damage)
{
	currentHealth -= damage;

	if (currentHealth > 0.0f) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(currentHealth));
	}
	else {
		//Dead
		currentHealth = 0.0f;
		bIsDead = true;
		DeathEvent.Broadcast();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Is dead (HealthComponent)"));
	}
}

void UHealthComponent::AlterHealth(float Amount)
{
	// already dead
	if (currentHealth <= 0)
		return;

	currentHealth = FMath::Clamp(currentHealth + Amount, 0.0f, maxHealth);

	UE_LOG(LogTemp, Log, TEXT("Health changed: %s (+%s)"), *FString::SanitizeFloat(currentHealth), *FString::SanitizeFloat(Amount));

	OnHealthChanged.Broadcast(this, currentHealth, -Amount, nullptr, nullptr, nullptr);

	bIsDead = currentHealth <= 0.0f;

	if (bIsDead) {
		DeathEvent.Broadcast();
	}
}

bool UHealthComponent::IsFriendly(AActor* actor1, AActor* actor2)
{
	if (actor1 == nullptr || actor2 == nullptr) {
		// assuming friendly
		return true;
	}

	UHealthComponent* healthCompA = Cast<UHealthComponent>(actor1->GetComponentByClass(UHealthComponent::StaticClass()));
	UHealthComponent* healthCompB = Cast<UHealthComponent>(actor2->GetComponentByClass(UHealthComponent::StaticClass()));

	if (healthCompA == nullptr || healthCompB == nullptr) {
		// assuming friendly
		return true;
	}

	return healthCompA->TeamNumber == healthCompB->TeamNumber;
}

