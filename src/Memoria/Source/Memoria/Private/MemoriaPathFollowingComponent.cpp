// Fill out your copyright notice in the Description page of Project Settings.


#include "MemoriaPathFollowingComponent.h"
#include "NavArea_Jump.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MemoriaNavigation.h"

void UMemoriaPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	Super::FollowPathSegment(DeltaTime);

}

void UMemoriaPathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);

	if (CharacterMoveComp == nullptr) {
		return;
	}

	const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];

	if (FNavAreaHelper::HasJumpFlag(SegmentStart)) {
		//CharacterMoveComp->SetMovementMode(MOVE_Flying);
		CharacterMoveComp->DoJump(true);
	}
	else {
		CharacterMoveComp->SetMovementMode(MOVE_Walking);
	}
}

void UMemoriaPathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
	Super::SetMovementComponent(MoveComp);

	CharacterMoveComp = Cast<UCharacterMovementComponent>(MoveComp);

}
