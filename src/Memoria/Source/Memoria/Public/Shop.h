// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

class UUserWidget;
class ABase;
class UBoxComponent;
class ACharacterBase;
class UShopWidget;
class APlayerControllerBase;
class UWeaponDataAsset;

UCLASS()
class MEMORIA_API AShop : public AActor
{
	GENERATED_BODY()
	
	friend UShopWidget;

public:	
	// Sets default values for this actor's properties
	AShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	UBoxComponent* EntryBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	UBoxComponent* ExitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> InteractWidgetClass;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* InteractWidgetInstance;

	UPROPERTY(BlueprintReadOnly)
	ACharacterBase* CurrentCharacter;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<UWeaponDataAsset*> WeaponsList;

	void ShowInteractWidget(APlayerControllerBase* playerController);

	void HideInteractWidget(APlayerControllerBase* playerController);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ShopWidgetClass;
	UPROPERTY(BlueprintReadOnly)
	UShopWidget* ShopWidgetInstance;

	UFUNCTION(BlueprintCallable)
	void ShowShopWidget(APlayerControllerBase* controller);
	UFUNCTION(BlueprintCallable)
	void HideShopWidget(APlayerControllerBase* controller);

	UFUNCTION()
	void ActorEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ActorExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void BuyWeaponForCharacter(UWeaponDataAsset* weapon, ACharacterBase* character);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool m_PlayerWithinBounds;
};
