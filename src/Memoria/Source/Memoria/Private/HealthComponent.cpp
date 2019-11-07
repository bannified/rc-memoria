// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Engine.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	DamageResist.BaseValue = 0.0f;

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	maxHealth = FModifiableAttribute(100.0f);
	MaxShields = FModifiableAttribute(0.0f);
	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();

	//if (GetOwnerRole() == ROLE_Authority)
	//{
		AActor* owner = GetOwner();
		if (owner)
		{
			owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
		}
	//}
		StartShieldCooldown();
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

	StartShieldCooldown();

	Damage *= FMath::Max(1.0f - DamageResist.GetValue(), 0.0f);

	float shieldsBefore = CurrentShields;

	CurrentShields = FMath::Clamp(CurrentShields - Damage, 0.0f, MaxShields.GetValue());
	float shieldsDamageTaken = shieldsBefore - CurrentShields;
	float leftover = Damage - shieldsDamageTaken;

	float healthBefore = currentHealth;

	currentHealth = FMath::Clamp(currentHealth - leftover, 0.0f, maxHealth.GetValue());

	float healthDamageTaken = healthBefore - currentHealth;

	OnHealthChanged.Broadcast(this, currentHealth, healthDamageTaken, DamageType, InstigatedBy, DamageCauser);
	OnShieldsChanged.Broadcast(this, CurrentShields, shieldsDamageTaken, DamageType, InstigatedBy, DamageCauser);

	OnOverallHealthChanged.Broadcast(this, currentHealth + CurrentShields, healthDamageTaken + shieldsDamageTaken, DamageType, InstigatedBy, DamageCauser);

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

void UHealthComponent::Init()
{
	currentHealth = startingHealth;
	CurrentShields = StartingShields;
	bIsDead = false;
}

void UHealthComponent::FullRestoreHealthComponent()
{
	currentHealth = maxHealth.GetValue();
	CurrentShields = MaxShields.GetValue();
	bIsDead = false;
}

void UHealthComponent::AlterHealth(float Amount)
{
	// already dead
	if (currentHealth <= 0)
		return;

	currentHealth = FMath::Clamp(currentHealth + Amount, 0.0f, maxHealth.GetValue());

	UE_LOG(LogTemp, Log, TEXT("Health changed: %s (+%s)"), *FString::SanitizeFloat(currentHealth), *FString::SanitizeFloat(Amount));

	OnHealthChanged.Broadcast(this, currentHealth, -Amount, nullptr, nullptr, nullptr);

	bIsDead = currentHealth <= 0.0f;

	if (bIsDead) {
		DeathEvent.Broadcast();
	}
}

void UHealthComponent::AlterShields(float Amount)
{
	CurrentShields = FMath::Clamp(CurrentShields + Amount, 0.0f, MaxShields.GetValue());

	UE_LOG(LogTemp, Log, TEXT("Shields changed: %s (+%s)"), *FString::SanitizeFloat(CurrentShields), *FString::SanitizeFloat(Amount));

	OnShieldsChanged.Broadcast(this, CurrentShields, -Amount, nullptr, nullptr, nullptr);
}

void UHealthComponent::StartShieldCooldown()
{
	GetWorld()->GetTimerManager().SetTimer(ShieldsTimer, this, &UHealthComponent::StartShieldRegen, ShieldRegenCooldown.GetValue(), false);
}

void UHealthComponent::StartShieldRegen()
{
	GetWorld()->GetTimerManager().SetTimer(ShieldsTimer, this, &UHealthComponent::ShieldRegenRoutine, ShieldsInterval, true, 0.0f);
}

void UHealthComponent::ShieldRegenRoutine()
{
	AlterShields(ShieldRegenValue.GetValue());
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

