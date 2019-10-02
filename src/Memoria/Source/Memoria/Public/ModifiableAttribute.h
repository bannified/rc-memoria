// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeModifier.h"
#include "ModifiableAttribute.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FModifiableAttribute
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifiableAttribute")
	float BaseValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ModifiableAttribute")
	TArray< FAttributeModifier > Mods;

	float GetValue()
	{
		if (m_IsDirty || m_LastBaseValue != BaseValue) {
			m_LastBaseValue = BaseValue;
			Value = ComputeFinalValue();
			m_IsDirty = false;
		}
		return Value;
	}

	FModifiableAttribute() : BaseValue(0.0f) {}
	FModifiableAttribute(float baseValue) : BaseValue(baseValue){};

	FORCEINLINE void AddModifierWithoutSort(const FAttributeModifier& mod)
	{
		m_IsDirty = true;
		Mods.Add(mod);
	}

	FORCEINLINE void AddModifier(const FAttributeModifier& mod)
	{
		AddModifierWithoutSort(mod);
		SortMods();
	}

	// Preserves Order
	FORCEINLINE void RemoveModifierSingle(const FAttributeModifier& mod)
	{
		m_IsDirty = true;
		Mods.RemoveSingle(mod);
	}

	// Preserves Order
	FORCEINLINE void RemoveModifiers(const FAttributeModifier& mod)
	{
		m_IsDirty = true;
		Mods.Remove(mod);
	}

	void SortMods()
	{
		m_IsDirty = true;
		Mods.Sort([](const FAttributeModifier& m1, const FAttributeModifier& m2)
		{
			if (m1.Type == m1.Type) {
				return m1.Order > m2.Order;
			}
			else {
				return m1.Type > m2.Type;
			}
		});
	}

protected:

	// Final value after applying all modifications to this Modifiable Attribute.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ModifiableAttribute")
	float Value;

private:

	float m_LastBaseValue = FLT_MIN;
	bool m_IsDirty = true;

	float ComputeFinalValue()
	{
		float finalValue = BaseValue;
		float sumPercentAdd = 0; // sum of percentage additive
		bool postPercentEnd = false;

		for (FAttributeModifier& mod : Mods)
		{
			switch (mod.Type) 
			{
				case EAttributeModType::Flat: // applied first
					finalValue += mod.Value;
					break;
				
				case EAttributeModType::PercentAdd: // applied second
					sumPercentAdd += mod.Value; // accumulating all the additives.
					break;
				case EAttributeModType::PercentMult: // applied last.
					if (!postPercentEnd) {
						finalValue *= 1 + sumPercentAdd;
						sumPercentAdd = 0;
						postPercentEnd = true;
					}
					finalValue *= 1 + mod.Value;
					break;
				default:
					break;
			}
		}

		if (!postPercentEnd) {
			finalValue *= 1 + sumPercentAdd;
		}

		return finalValue;
	}
};
