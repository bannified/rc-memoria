// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "MemoriaPathFollowingComponent.generated.h"

class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class MEMORIA_API UMemoriaPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()

protected:
	/* cached CharacterMovementComponent */
	UPROPERTY(transient)
	UCharacterMovementComponent* CharacterMoveComp;

public:

	virtual void FollowPathSegment(float DeltaTime) override;

	// used to detect properties of a path's segment a character is about to follow
	virtual void SetMoveSegment(int32 SegmentStartIndex) override;

	// used to cache UCharacterMovementComponent we're using in SetMoveSegment
	virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;
	
};
