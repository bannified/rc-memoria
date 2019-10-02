// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealNetwork.h"
#include "Engine/GameEngine.h"

/// Collision Channels
#define COLLISION_AIM_DETECT			ECC_GameTraceChannel1
#define COLLISION_PROJECTILE			ECC_GameTraceChannel2
#define COLLISION_UNIT					ECC_GameTraceChannel3
#define COLLISION_PROJECTILEAOEBLOCK	ECC_GameTraceChannel4

// Legacy
#define COLLISION_BASE					ECC_GameTraceChannel2
#define COLLISION_WATER					ECC_GameTraceChannel3

// Print macros
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
#define PRINT_INFO(...) if(GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(__VA_ARGS__))); }
#else
#define PRINT_INFO(x)
#endif
