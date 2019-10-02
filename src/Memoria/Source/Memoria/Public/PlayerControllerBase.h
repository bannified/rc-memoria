// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerControllerBase.generated.h"

class ACharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerControllerInput, APlayerControllerBase*, character);

/**
 * 
 */
UCLASS()
class MEMORIA_API APlayerControllerBase : public APlayerController, public IGenericTeamAgentInterface
{

	GENERATED_BODY()
public:
	APlayerControllerBase();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

protected:
	/* Axis-based Input */
	virtual void MoveForward(float value);
	virtual void MoveRight(float value);
	virtual void MoveUp(float value);

	virtual void LookUp(float value);
	virtual void LookRight(float value);

	/* Action Inputs */

		/* Gameplay */
	virtual void PrimaryFireStart();
	virtual void PrimaryFireEnd();
	virtual void SecondaryFireStart();
	virtual void SecondaryFireEnd();

	virtual void ReloadStart();
	virtual void ReloadEnd();

	virtual void JumpStart();
	virtual void JumpEnd();
	virtual void MovementModStart();
	virtual void MovementModEnd();
	virtual void InteractStart();
	virtual void InteractEnd();

	virtual void Special1Start();
	virtual void Special1End();

	virtual void Special2Start();
	virtual void Special2End();

	virtual void PreviousEquipment();
	virtual void NextEquipment();

		/* General */
	virtual void Escape();
	virtual void Contextual();
	virtual void Confirm();
	virtual void Pause();

		/* Shop */
	virtual void ShopBuy();

		/* UI */
	virtual void UILeft();
	virtual void UIRight();
	virtual void UIUp();
	virtual void UIDown();

	virtual void OnPossess(APawn* aPawn) override;

	virtual void OnUnPossess() override;

public:

	inline void SetInputToCharacter(bool active) { m_IsInputToCharacterActive = active; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ACharacterBase* GetCharacterBase() { return m_Character; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerControllerInput OnEscapeDown; 

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerControllerInput OnPreviousEquipment; 
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerControllerInput OnNextEquipment;

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnPlayerControllerInput OnJumpStart;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnPlayerControllerInput OnJumpEnd;

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnPlayerControllerInput OnInteractStart;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnPlayerControllerInput OnInteractEnd;

	UPROPERTY(BlueprintAssignable, Category = "PlayerControllerBase|Events")
	FOnPlayerControllerInput OnSpecial1Start;
	UPROPERTY(BlueprintAssignable, Category = "PlayerControllerBase|Events")
	FOnPlayerControllerInput OnSpecial1End;

	UPROPERTY(BlueprintAssignable, Category = "PlayerControllerBase|Events")
	FOnPlayerControllerInput OnSpecial2Start;
	UPROPERTY(BlueprintAssignable, Category = "PlayerControllerBase|Events")
	FOnPlayerControllerInput OnSpecial2End;

	/* Shop */

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnPlayerControllerInput OnUILeft;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnPlayerControllerInput OnUIRight;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnPlayerControllerInput OnUIUp;
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnPlayerControllerInput OnUIDown;

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FOnPlayerControllerInput OnShopBuy;

protected:
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Player")
	ACharacterBase* m_Character;
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Player")
	bool m_IsInputToCharacterActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	uint8 TeamId;

	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

};