// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopWidget.h"
#include "CharacterBase.h"
#include "Shop.h"
#include "PlayerControllerBase.h"

void UShopWidget::Setup(APlayerControllerBase* controller, AShop* shop)
{
	controller->OnUILeft.AddDynamic(this, &UShopWidget::OnLeftButton);
	controller->OnUIRight.AddDynamic(this, &UShopWidget::OnRightButton);
	controller->OnUIUp.AddDynamic(this, &UShopWidget::OnUpButton);
	controller->OnUIDown.AddDynamic(this, &UShopWidget::OnDownButton);

	controller->OnEscapeDown.AddDynamic(this, &UShopWidget::OnEscapeButton);
	controller->OnShopBuy.AddDynamic(this, &UShopWidget::OnShopBuy);
	
	ShopInstance = shop;

	OnSetupReceive();
}

void UShopWidget::Shutdown(APlayerControllerBase* controller)
{
	controller->OnUILeft.RemoveDynamic(this, &UShopWidget::OnLeftButton);
	controller->OnUIRight.RemoveDynamic(this, &UShopWidget::OnRightButton);
	controller->OnUIUp.RemoveDynamic(this, &UShopWidget::OnUpButton);
	controller->OnUIDown.RemoveDynamic(this, &UShopWidget::OnDownButton);

	controller->OnEscapeDown.RemoveDynamic(this, &UShopWidget::OnEscapeButton);
	controller->OnShopBuy.RemoveDynamic(this, &UShopWidget::OnShopBuy);

	ShopInstance = nullptr;

	OnShutdownReceive();
}

bool UShopWidget::BuyWeapon(UWeaponDataAsset* weapon)
{
	ACharacterBase* character = ShopInstance->CurrentCharacter;

	if (weapon->IsBuyableByCharacter(character)) {
		ShopInstance->BuyWeaponForCharacter(weapon, character);
		return true;
	}
	else {
		return false;
	}
}

void UShopWidget::OnLeftButton(APlayerControllerBase* controller)
{
	OnLeftButtonReceive.Broadcast(controller);
}

void UShopWidget::OnRightButton(APlayerControllerBase* controller)
{
	OnRightButtonReceive.Broadcast(controller);
}

void UShopWidget::OnDownButton(APlayerControllerBase* controller)
{
	OnDownButtonReceive.Broadcast(controller);
}

void UShopWidget::OnUpButton(APlayerControllerBase* controller)
{
	OnUpButtonReceive.Broadcast(controller);
}

void UShopWidget::OnEscapeButton(APlayerControllerBase* controller)
{
	OnEscapeButtonReceive.Broadcast(controller);
}

void UShopWidget::OnShopBuy(APlayerControllerBase* controller)
{
	OnShopBuyReceive.Broadcast(controller);
}
