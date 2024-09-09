// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherPhysicsTypes.generated.h"

/**
 * Defines how a physics object is controlled within the Tether physics engine.
 */
UENUM(BlueprintType)
enum class ETetherSimulationMode : uint8
{
	Simulated			UMETA(ToolTip="Simulated by the physics engine"),
	Kinematic			UMETA(ToolTip="Does not respond to physics forces"),
	Inertial			UMETA(ToolTip="Retains internal physics like damping but won't apply external forces"),
};

/**
 * Defines the wake/sleep state of a physics object, determining whether it's actively simulated or idle.
 */
UENUM(BlueprintType)
enum class ETetherActivityState : uint8
{
	Awake				UMETA(ToolTip="Awake and actively simulated"),
	ForceAwake			UMETA(ToolTip="Awake and actively simulated - will not sleep"),
	Asleep				UMETA(ToolTip="Asleep and not simulated until disturbed"),
	ForceAsleep			UMETA(ToolTip="Asleep and not simulated even if disturbed - will not wake"),
};