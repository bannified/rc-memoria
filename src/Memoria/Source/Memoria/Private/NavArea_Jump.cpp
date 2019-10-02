// Fill out your copyright notice in the Description page of Project Settings.


#include "NavArea_Jump.h"
#include "MemoriaNavigation.h"

UNavArea_Jump::UNavArea_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FNavAreaHelper::SetFlag(AreaFlags, ENavAreaFlag::Jump);
}
