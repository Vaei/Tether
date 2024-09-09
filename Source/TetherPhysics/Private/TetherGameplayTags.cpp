// Copyright (c) Jared Taylor. All Rights Reserved


#include "TetherGameplayTags.h"

namespace FTetherGameplayTags
{
	/** Gameplay tags for tether hashing systems */
	UE_DEFINE_GAMEPLAY_TAG(Tether_Hashing, "Tether.Hashing");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Hashing_Spatial, "Tether.Hashing.Spatial");

	/** Gameplay tags for tether detection systems */
	UE_DEFINE_GAMEPLAY_TAG(Tether_Detection_BroadPhase, "Tether.Detection.BroadPhase");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Detection_NarrowPhase, "Tether.Detection.NarrowPhase");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Detection_CollisionHandler, "Tether.Detection.CollisionHandler");

	/** Gameplay tags for tether activity state handlers */
	UE_DEFINE_GAMEPLAY_TAG(Tether_ActivityState, "Tether.ActivityState");
	
	/** Gameplay tags for tether physics solvers */
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Linear, "Tether.Solver.Linear");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Angular, "Tether.Solver.Angular");

	/** Gameplay tags for tether integration solvers */
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Integration, "Tether.Solver.Integration");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Integration_Euler, "Tether.Solver.Integration.Euler");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Integration_RK4, "Tether.Solver.Integration.RK4");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Integration_Verlet, "Tether.Solver.Integration.Verlet");
	
	/** Gameplay tags for tether contact solvers */
	UE_DEFINE_GAMEPLAY_TAG(Tether_Solver_Contact, "Tether.Solver.Contact");

	/** Gameplay tags for tether replay system */
	UE_DEFINE_GAMEPLAY_TAG(Tether_Replay, "Tether.Replay");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Replay_Mode_Idle, "Tether.Replay.Mode.Idle");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Replay_Mode_Recording, "Tether.Replay.Mode.Recording");
	UE_DEFINE_GAMEPLAY_TAG(Tether_Replay_Mode_Replaying, "Tether.Replay.Mode.Replaying");

	/** Gameplay tags for tether shapes */
	UE_DEFINE_GAMEPLAY_TAG(Tether_Shape, "Tether.Shape");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_AxisAlignedBoundingBox, "Tether.Shape.AxisAlignedBoundingBox", "AABBs are simpler to compute, as they only require finding the minimum and maximum extents of the object along the coordinate axes. Because they are axis-aligned, checking for overlaps between two AABBs is computationally cheap");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_OrientedBoundingBox, "Tether.Shape.OrientedBoundingBox", "Checking for overlaps between two OBBs is more computationally expensive than AABB, often requiring separating axis tests or other geometric methods.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_BoundingSphere, "Tether.Shape.BoundingSphere", "The Bounding Sphere is extremely simple to compute. It is defined by a center point and a radius, which can be derived from the furthest point from the center of the object.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_Capsule, "Tether.Shape.Capsule", "Capsules are more complex than spheres due to their elongated shape, but they are simpler than boxes (OBB) when it comes to collision detection. Collision detection for capsules typically involves checking both the cylindrical part and the hemispherical ends, which is more complex than sphere collision detection but still relatively efficient.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Shape_Pipe, "Tether.Shape.Pipe", "Pipes are more complex than capsules and spheres due to their hollow cylindrical shape with adjustable arc angles, but they are simpler than boxes (OBB) when it comes to collision detection. Collision detection for pipes involves checking both the inner and outer surfaces, as well as accounting for the specified arc, making it more complex than capsule or sphere detection but still relatively efficient.");
}