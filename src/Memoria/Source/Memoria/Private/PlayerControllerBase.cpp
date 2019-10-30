// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"
#include "CharacterBase.h"
#include "Memoria.h"
#include "GameTeam.h"

APlayerControllerBase::APlayerControllerBase()
{
	m_IsInputToCharacterActive = true;
	TeamId = 1;
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &APlayerControllerBase::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerControllerBase::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &APlayerControllerBase::MoveUp);

	InputComponent->BindAxis("LookUp", this, &APlayerControllerBase::LookUp);
	InputComponent->BindAxis("LookRight", this, &APlayerControllerBase::LookRight);

	InputComponent->BindAction("Attack0", EInputEvent::IE_Pressed, this, &APlayerControllerBase::PrimaryFireStart);
	InputComponent->BindAction("Attack0", EInputEvent::IE_Released, this, &APlayerControllerBase::PrimaryFireEnd);

	InputComponent->BindAction("Attack1", EInputEvent::IE_Pressed, this, &APlayerControllerBase::SecondaryFireStart);
	InputComponent->BindAction("Attack1", EInputEvent::IE_Released, this, &APlayerControllerBase::SecondaryFireEnd);

	InputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &APlayerControllerBase::ReloadStart);
	InputComponent->BindAction("Reload", EInputEvent::IE_Released, this, &APlayerControllerBase::ReloadEnd);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &APlayerControllerBase::JumpStart);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &APlayerControllerBase::JumpEnd);

	InputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &APlayerControllerBase::InteractStart);
	InputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &APlayerControllerBase::InteractEnd);

	InputComponent->BindAction("Special1", EInputEvent::IE_Pressed, this, &APlayerControllerBase::Special1Start);
	InputComponent->BindAction("Special1", EInputEvent::IE_Released, this, &APlayerControllerBase::Special1End);

	InputComponent->BindAction("Special2", EInputEvent::IE_Pressed, this, &APlayerControllerBase::Special2Start);
	InputComponent->BindAction("Special2", EInputEvent::IE_Released, this, &APlayerControllerBase::Special2End);

	InputComponent->BindAction("PreviousEquipment", EInputEvent::IE_Pressed, this, &APlayerControllerBase::PreviousEquipment);
	InputComponent->BindAction("NextEquipment", EInputEvent::IE_Pressed, this, &APlayerControllerBase::NextEquipment);

	InputComponent->BindAction("Mobility", EInputEvent::IE_Pressed, this, &APlayerControllerBase::MovementModStart);
	InputComponent->BindAction("Mobility", EInputEvent::IE_Released, this, &APlayerControllerBase::MovementModEnd);

	InputComponent->BindAction("Escape", EInputEvent::IE_Pressed, this, &APlayerControllerBase::Escape);
	InputComponent->BindAction("Confirm", EInputEvent::IE_Pressed, this, &APlayerControllerBase::Confirm);
	InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &APlayerControllerBase::Pause);
	InputComponent->BindAction("Contextual", EInputEvent::IE_Pressed, this, &APlayerControllerBase::Contextual);

	InputComponent->BindAction("Shop_Buy", EInputEvent::IE_Pressed, this, &APlayerControllerBase::ShopBuy);
	InputComponent->BindAction("UILeft", EInputEvent::IE_Pressed, this, &APlayerControllerBase::UILeft);
	InputComponent->BindAction("UIRight", EInputEvent::IE_Pressed, this, &APlayerControllerBase::UIRight);
	InputComponent->BindAction("UIUp", EInputEvent::IE_Pressed, this, &APlayerControllerBase::UIUp);
	InputComponent->BindAction("UIDown", EInputEvent::IE_Pressed, this, &APlayerControllerBase::UIDown);
}

void APlayerControllerBase::MoveForward(float value)
{
	if (m_Character)
	{
		m_Character->MoveForward(value);
	}
}

void APlayerControllerBase::MoveRight(float value)
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->MoveRight(value);
	}
}

void APlayerControllerBase::MoveUp(float value)
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->MoveUp(value);
	}
}

void APlayerControllerBase::LookUp(float value)
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->AddControllerPitchInput(value);
	}
}

void APlayerControllerBase::LookRight(float value)
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->AddControllerYawInput(value);
	}
}

void APlayerControllerBase::PrimaryFireStart()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->PrimaryFireStart();
		UE_LOG(LogTemp, Log, TEXT("PrimaryFireStart"));
	}
}

void APlayerControllerBase::PrimaryFireEnd()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->PrimaryFireEnd();
		UE_LOG(LogTemp, Log, TEXT("PrimaryFireEnd"));

	}
}

void APlayerControllerBase::SecondaryFireStart()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->SecondaryFireStart();
	}
}

void APlayerControllerBase::SecondaryFireEnd()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->SecondaryFireEnd();
	}
}

void APlayerControllerBase::ReloadStart()
{
	if (m_IsInputToCharacterActive && m_Character) {
		m_Character->ReloadStart();
	}
}

void APlayerControllerBase::ReloadEnd()
{
	if (m_IsInputToCharacterActive && m_Character) {
		m_Character->ReloadEnd();
	}
}

void APlayerControllerBase::JumpStart()
{
	if (m_IsInputToCharacterActive && m_Character) {
		m_Character->JumpStart();
	}
}

void APlayerControllerBase::JumpEnd()
{
	if (m_IsInputToCharacterActive && m_Character) {
		m_Character->JumpEnd();
	}
}

void APlayerControllerBase::MovementModStart()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->MovementModStart();
		UE_LOG(LogTemp, Log, TEXT("MovementModStart"));

	}
}

void APlayerControllerBase::MovementModEnd()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->MovementModEnd();
	}
}

void APlayerControllerBase::InteractStart()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->InteractStart();
		OnInteractStart.Broadcast(this);
	}
}

void APlayerControllerBase::InteractEnd()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->InteractEnd();
		OnInteractEnd.Broadcast(this);
	}
}

void APlayerControllerBase::Special1Start()
{
	if (m_IsInputToCharacterActive && m_Character) {
		m_Character->Special1Start();
		OnSpecial1Start.Broadcast(this);
	}
}

void APlayerControllerBase::Special1End()
{
	if (m_IsInputToCharacterActive && m_Character) {
		m_Character->Special1End();
		OnSpecial1End.Broadcast(this);
	}
}

void APlayerControllerBase::Special2Start()
{
	if (m_IsInputToCharacterActive && m_Character) {
		m_Character->Special2Start();
		OnSpecial2Start.Broadcast(this);
	}
}

void APlayerControllerBase::Special2End()
{
	if (m_IsInputToCharacterActive && m_Character) {
		m_Character->Special2End();
		OnSpecial2End.Broadcast(this);
	}
}

void APlayerControllerBase::PreviousEquipment()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->PreviousEquipment();
	}

	OnPreviousEquipment.Broadcast(this);
}

void APlayerControllerBase::NextEquipment()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->NextEquipment();
	}

	OnNextEquipment.Broadcast(this);
}

void APlayerControllerBase::Escape()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->Escape();
	}

	OnEscapeDown.Broadcast(this);
}

void APlayerControllerBase::Contextual()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->Contextual();
	}
}

void APlayerControllerBase::Confirm()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->Confirm();
	}
}

void APlayerControllerBase::Pause()
{
	if (m_IsInputToCharacterActive && m_Character)
	{
		m_Character->Pause();
	}
}

void APlayerControllerBase::ShopBuy()
{
	OnShopBuy.Broadcast(this);
}

void APlayerControllerBase::UILeft()
{
	OnUILeft.Broadcast(this);
}

void APlayerControllerBase::UIRight()
{
	OnUIRight.Broadcast(this);
}

void APlayerControllerBase::UIUp()
{
	OnUIUp.Broadcast(this);
}

void APlayerControllerBase::UIDown()
{
	OnUIDown.Broadcast(this);
}

void APlayerControllerBase::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	//PRINT_INFO("Possessed called in ACharacterBase in PC: %s", *(this->GetName()));

	ACharacterBase* character = Cast<ACharacterBase>(GetPawn());
	if (character != nullptr)
	{
		m_Character = character;
		m_Character->PossessedByPlayerControllerBase(this);
	}
	else
	{
		PRINT_INFO("Possessing with no ACharacterBase in PC: %s", *(this->GetName()));
	}
}

void APlayerControllerBase::OnUnPossess()
{
	ACharacterBase* character = Cast<ACharacterBase>(GetPawn());
	if (character != nullptr) {
		m_Character = character;
		m_Character->UnPossessedByPlayerControllerBase(this);
	}

	Super::OnUnPossess();
}

void APlayerControllerBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	TeamId = (uint8)(EGameTeam)TeamID.GetId();
}

FGenericTeamId APlayerControllerBase::GetGenericTeamId() const
{
	return uint8(TeamId);

}

void APlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerControllerBase, m_Character);
	DOREPLIFETIME(APlayerControllerBase, m_IsInputToCharacterActive);

}
