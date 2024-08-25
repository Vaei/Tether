// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TetherThreadTypes.generated.h"

/**
 * Enum for controlling where the physics update occurs.
 */
UENUM(BlueprintType)
enum class ETetherPhysicsUpdateMode : uint8
{
	GameThread			UMETA(ToolTip="Update in the main game thread. For simpler or non-vital simulations."),
	SingleThread		UMETA(ToolTip="Update in a single custom thread. For improved determinism at a performance cost."),
	MultiThread			UMETA(ToolTip="Update in a multi-threaded setup. For improved performance at the cost of determinism. May be more expensive than game thread update for simple simulations.")
};