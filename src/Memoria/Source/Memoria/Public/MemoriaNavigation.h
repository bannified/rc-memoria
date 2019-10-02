#pragma once

#include "NavMeshPath.h"

UENUM()
namespace ENavAreaFlag
{
	// only up to 15 values allowed. first bit is reserved for walkable/not walkable
	enum Type
	{
		Default,
		Jump,
		Crouch,
		// more...
	};
}

namespace FNavAreaHelper
{
	FORCEINLINE bool IsFlagSet(uint16 Flags, ENavAreaFlag::Type Bit) { return (Flags & (1 << Bit)) != 0; }
	FORCEINLINE void SetFlag(uint16& Flags, ENavAreaFlag::Type Bit) { (Flags |= (1 << Bit)); }

	FORCEINLINE bool IsNavLink(const FNavPathPoint& PathVert) 
	{ 
		return (FNavMeshNodeFlags(PathVert.Flags).PathFlags & RECAST_STRAIGHTPATH_OFFMESH_CONNECTION) != 0;
	}
	FORCEINLINE bool HasJumpFlag(const FNavPathPoint& PathVert)
	{
		return IsFlagSet(FNavMeshNodeFlags(PathVert.Flags).AreaFlags, ENavAreaFlag::Jump);
	}
	FORCEINLINE bool HasCrouchFlag(const FNavPathPoint& PathVert)
	{
		return IsFlagSet(FNavMeshNodeFlags(PathVert.Flags).AreaFlags, ENavAreaFlag::Crouch);
	}
}

