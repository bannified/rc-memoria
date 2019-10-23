// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterPerkComponent.generated.h"


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract )
class MEMORIA_API UCharacterPerkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterPerkComponent();

	UFUNCTION(BlueprintCallable, Category = "CharacterPerk")
	virtual void Setup(ACharacterBase* character);
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterPerk")
	void BP_Setup(ACharacterBase* character);

	UFUNCTION(BlueprintCallable, Category = "CharacterPerk")
	virtual void Teardown(ACharacterBase* character);
	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterPerk")
	void BP_Teardown(ACharacterBase* character);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
