// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnUnit.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "DelegateSignatureImpl.inl"
#include "SpawnUnitQueryInstanceWrapper.generated.h"

class AWaveGameModeState;

/**
 * 
 */
UCLASS()
class MEMORIA_API USpawnUnitQueryInstanceWrapper : public UEnvQueryInstanceBlueprintWrapper
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSpawnUnit SpawnUnit;

	UPROPERTY(BlueprintAssignable, Category = "AI|EQS", meta = (DisplayName = "OnQueryDone"))
	FEQSQueryDoneSignature OnQueryDoneEvent;

	template< class UserClass >
	void BindDelegateToQueryDoneEvent(UserClass* InUserObject, typename TBaseDynamicDelegate<FWeakObjectPtr, void, UEnvQueryInstanceBlueprintWrapper *, EEnvQueryStatus::Type>::TMethodPtrResolver<UserClass>::FMethodPtr InMethodPtr);

	template< class UserClass >
	void RemoveDelegateFromQueryDoneEvent(UserClass* InUserObject, typename TBaseDynamicDelegate<FWeakObjectPtr, void, UEnvQueryInstanceBlueprintWrapper *, EEnvQueryStatus::Type>::TMethodPtrResolver<UserClass>::FMethodPtr InMethodPtr);

	template< class UserClass >
	void BindWaveStateSpawnMethod(UserClass* InUserObject);

	template< class UserClass >
	void RemoveWaveStateSpawnMethod(UserClass* InUserObject);
};

template< class UserClass >
void USpawnUnitQueryInstanceWrapper::BindDelegateToQueryDoneEvent(UserClass* InUserObject, typename TBaseDynamicDelegate<FWeakObjectPtr, void, UEnvQueryInstanceBlueprintWrapper *, EEnvQueryStatus::Type>::TMethodPtrResolver<UserClass>::FMethodPtr InMethodPtr)
{
	OnQueryDoneEvent.AddDynamic(InUserObject, InMethodPtr);
}

template< class UserClass >
void USpawnUnitQueryInstanceWrapper::RemoveDelegateFromQueryDoneEvent(UserClass* InUserObject, typename TBaseDynamicDelegate<FWeakObjectPtr, void, UEnvQueryInstanceBlueprintWrapper *, EEnvQueryStatus::Type>::TMethodPtrResolver<UserClass>::FMethodPtr InMethodPtr)
{
	OnQueryDoneEvent.RemoveDynamic(InUserObject, InMethodPtr);
}

template< class UserClass >
void USpawnUnitQueryInstanceWrapper::BindWaveStateSpawnMethod(UserClass* InUserObject)
{
	OnQueryDoneEvent.AddDynamic(InUserObject, &AWaveGameModeState::OnSpawnGroupQueryFinished);
}

template< class UserClass >
void USpawnUnitQueryInstanceWrapper::RemoveWaveStateSpawnMethod(UserClass* InUserObject)
{
	OnQueryDoneEvent.RemoveDynamic(InUserObject, &AWaveGameModeState::OnSpawnGroupQueryFinished);
}
