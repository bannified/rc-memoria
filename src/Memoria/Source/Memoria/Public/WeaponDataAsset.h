// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponProperties.h"
#include "SlateBrush.h"
#include "WeaponDataAsset.generated.h"

class UStaticMesh;
class ANozzleBase;
class ACharacterBase;

/**
 * 
 */
UCLASS()
class MEMORIA_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	TArray<FWeaponProperties> PropertiesList;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetMaxLevel() { return PropertiesList.Num(); }

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Gameplay")
	TSubclassOf<ANozzleBase> NozzleClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Visual")
	UStaticMesh* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Description")
	FName DisplayName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Description")
	FSlateBrush DisplayImage;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool IsBuyableByCharacter(ACharacterBase* character);

};
