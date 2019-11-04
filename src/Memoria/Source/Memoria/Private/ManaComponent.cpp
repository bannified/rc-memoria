// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaComponent.h"
#include "CharacterBase.h"

// Sets default values for this component's properties
UManaComponent::UManaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	MaxMana = FModifiableAttribute(100.0f);
	ReloadTime = FModifiableAttribute(1.2f);
}


// Called when the game starts
void UManaComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character == nullptr) {
		ACharacterBase* castedOwner = Cast<ACharacterBase>(GetOwner());
		Character = castedOwner;
	}

	ModifyMana(MaxMana.GetValue());
	
}


// Called every frame
void UManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UManaComponent::ModifyMana(float value, bool overflow)
{
	CurrentMana += value;
	if (!overflow) {
		CurrentMana = FMath::Min(CurrentMana, MaxMana.GetValue());
	}
	
	if (CurrentMana == 0) {
		StartReload();
	}

	OnManaChangeEvent.Broadcast(Character, this);
}

void UManaComponent::StartReload()
{
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &UManaComponent::ReloadDone, ReloadTime.GetValue(), false, -1.f);

	ReloadStartEvent.Broadcast(Character, this);

}

void UManaComponent::InterruptReload()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);

	ReloadInterruptEvent.Broadcast(Character, this);
}

void UManaComponent::ReloadDone()
{
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);

	ModifyMana(MaxMana.GetValue());

	ReloadCompleteEvent.Broadcast(Character, this);

}

void UManaComponent::FullRestoreMana()
{
	ModifyMana(MaxMana.GetValue(), false);
}

