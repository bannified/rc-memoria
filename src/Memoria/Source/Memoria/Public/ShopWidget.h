// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shop.h"
#include "ShopWidget.generated.h"

class ACharacterBase;
class APlayerControllerBase;
class AShop;
class UWeaponDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerInputEvent, APlayerControllerBase*, controller);

/**
 * 
 */
UCLASS()
class MEMORIA_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()

	friend AShop;

public:
	void Setup(APlayerControllerBase* controller, AShop* shop);
	void Shutdown(APlayerControllerBase* controller);

protected:

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FPlayerInputEvent OnLeftButtonReceive;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FPlayerInputEvent OnRightButtonReceive;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FPlayerInputEvent OnUpButtonReceive;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FPlayerInputEvent OnDownButtonReceive;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FPlayerInputEvent OnEscapeButtonReceive;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FPlayerInputEvent OnShopBuyReceive;

	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool BuyWeapon(UWeaponDataAsset* weapon);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shop")
	AShop* ShopInstance;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetupReceive();
	UFUNCTION(BlueprintImplementableEvent)
	void OnShutdownReceive();

private:

	UFUNCTION()
	void OnLeftButton(APlayerControllerBase* controller);
	UFUNCTION()
	void OnRightButton(APlayerControllerBase* controller);
	UFUNCTION()
	void OnDownButton(APlayerControllerBase* controller);
	UFUNCTION()
	void OnUpButton(APlayerControllerBase* controller);

	UFUNCTION()
	void OnEscapeButton(APlayerControllerBase* controller);

	UFUNCTION()
	void OnShopBuy(APlayerControllerBase* controller);
};
