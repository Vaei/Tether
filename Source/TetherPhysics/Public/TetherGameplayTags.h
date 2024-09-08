// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

/**
 * Namespace containing all the gameplay tags used in the Tether physics system.
 * 
 * These tags are used to categorize and identify various components within the Tether system,
 * such as hashing methods, solvers, and shape types. By organizing them into gameplay tags,
 * you can easily manage and reference these components throughout the codebase.
 */
namespace FTetherGameplayTags
{
	/** Gameplay tags for tether hashing */
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Hashing);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Hashing_Spatial);

	/** Gameplay tags for tether detection and collision systems */
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Detection_BroadPhase);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Detection_NarrowPhase);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Detection_CollisionHandler);

	/** Gameplay tags for tether physics solvers */
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Linear);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Angular);

	/** Gameplay tags for tether integration solvers */
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Integration);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Integration_Euler);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Integration_RK4);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Integration_Verlet);

	/** Gameplay tags for tether contact solvers */
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Contact);

	/** Gameplay tags for tether replay system */
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Replay);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Replay_Mode_Idle);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Replay_Mode_Recording);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Replay_Mode_Replaying);
	
	/** Gameplay tags for tether shapes */
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_AxisAlignedBoundingBox);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_OrientedBoundingBox);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_BoundingSphere);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_Capsule);
	TETHERPHYSICS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_Pipe);
}