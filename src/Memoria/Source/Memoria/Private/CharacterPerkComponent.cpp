// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterPerkComponent.h"

// Sets default values for this component's properties
UCharacterPerkComponent::UCharacterPerkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCharacterPerkComponent::Setup(ACharacterBase* character)
{
	BP_Setup(character);
}

void UCharacterPerkComponent::Teardown(ACharacterBase* character)
{
	BP_Teardown(character);
	DestroyComponent();
}

// Called when the game starts
void UCharacterPerkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterPerkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

