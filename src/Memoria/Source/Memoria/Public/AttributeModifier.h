// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeModifier.generated.h"

UENUM(BlueprintType)
enum class EAttributeModType : uint8
{
	Flat = 1, // Flat addition
	PercentAdd = 2, // Additive Percent
	PercentMult = 3, // Multiplicative
};


/**
 * 
 */
USTRUCT(BlueprintType)
struct FAttributeModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeModifier")
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeModifier")
	int Order; // order at which this attribute gets applied.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeModifier")
	EAttributeModType Type = EAttributeModType::Flat;

	FAttributeModifier()
	{
		Value = 0;
		Order = 0;
		Type = EAttributeModType::Flat;
	};

	FAttributeModifier(float Value, int Order, EAttributeModType Type)
		: Value(Value), Order(Order), Type(Type) {}


	// Overload the == operator so that it can work with Array's Find.
	bool operator ==(const FAttributeModifier& rhs) const
	{
		return Type == rhs.Type && Value == rhs.Value && Order == rhs.Order;
	}
};

