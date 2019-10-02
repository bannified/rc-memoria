#pragma once

#include "WeaponProperties.generated.h"

USTRUCT(BlueprintType)
struct FWeaponProperties
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PrimaryPower;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PrimaryCooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float PrimaryDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SecondaryPower;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SecondaryCooldown;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SecondaryDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RateOfFire;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Mobility;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CostOfUpgrade;
};