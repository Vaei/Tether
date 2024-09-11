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
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Solver_Contact_RigidBody_ImpulseVelocityLevel, "Tether.Solver.Contact.RigidBody.ImpulseVelocityLevel", "Operates on the velocity level, using impulse-based resolution techniques to prevent interpenetration and handle collisions between rigid bodies. It calculates the appropriate changes in velocity to ensure physically accurate reactions, such as bouncing, sliding, or resting contacts.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Solver_Contact_RigidBody_ProjectedGaussSeidel, "Tether.Solver.Contact.RigidBody.ProjectedGaussSeidel", "Uses the Projected Gauss-Seidel (PGS) method, an iterative approach for resolving contacts and constraints between rigid bodies. It projects constraints into valid positions while resolving interpenetration, velocity corrections, and friction between objects. PGS is often used in real-time systems for its stability and efficiency.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Solver_Contact_RigidBody_SequentialImpulse, "Tether.Solver.Contact.RigidBody.SequentialImpulse", "Employs the Sequential Impulse (SI) method, resolving rigid body contacts by applying impulses iteratively. It computes velocity-level changes at each contact point, ensuring that objects respond correctly to collisions, friction, and restitution. SI is used in many real-time physics engines due to its performance benefits in rigid body dynamics.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_Solver_Contact_RigidBody_Iterative, "Tether.Solver.Contact.RigidBody.Iterative", "Uses a general iterative approach to resolve contact and collision responses between rigid bodies. By iterating over each contact constraint, it ensures that collisions are resolved step-by-step, preventing interpenetration and applying friction and restitution forces. This solver is suitable for systems requiring a balance between performance and accuracy.");

	/** Gameplay tags for tether post-solve corrections */
	UE_DEFINE_GAMEPLAY_TAG(Tether_PostSimulation, "Tether.PostSimulation");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_PostProjection, "Tether.PostSimulation.PostProjection", "Applies post-projection corrections to resolve small positional errors caused by floating-point inaccuracies, constraint violations, or penetration drift. This technique moves objects directly into valid positions after the contact and constraint solvers have run.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_VelocityDamping, "Tether.PostSimulation.VelocityDamping", "Adds velocity damping to slow down objects and prevent them from accumulating excess kinetic energy. This is useful for applying drag forces (e.g., air resistance) or for stabilizing systems by reducing small, unwanted velocities over time.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_VelocityClamping, "Tether.PostSimulation.VelocityClamping", "Limits the maximum velocity that objects can reach during the simulation. This helps to prevent objects from moving too fast or tunneling through other objects, especially when high-speed interactions are involved.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_ErrorCorrection, "Tether.PostSimulation.ErrorCorrection", "Focuses on fixing any numerical errors that may have built up during the simulation. This technique can address small inconsistencies in object positions, orientations, or velocities by applying corrections based on the accumulated error.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_RestingStateDetection, "Tether.PostSimulation.RestingStateDetection", "Detects when objects are at rest or nearly at rest (e.g., minimal velocity or acceleration) and stabilizes their position. This can include setting objects to a sleep state to save performance or ensuring they don't vibrate or jitter.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_ConstraintDriftCorrection, "Tether.PostSimulation.ConstraintDriftCorrection", "Fixes any small drifts or violations in constraints (like joints or hinges) that can accumulate during the simulation. This ensures that constraints are respected after the main solver steps have finished.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_ImpulseDamping, "Tether.PostSimulation.ImpulseDamping", "Applies a damping effect to impulses generated during the contact solver, reducing excessive bouncing or jittering. This technique ensures that impulses don’t create too much instability in the system.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_FrictionStabilization, "Tether.PostSimulation.FrictionStabilization", "Ensures that objects that should be resting or sliding based on friction are stabilized after the main solver steps. This can prevent jittery behavior where objects are supposed to be at rest due to friction but keep moving slightly.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_ConstraintRelaxation, "Tether.PostSimulation.ConstraintRelaxation", "Gradually relaxes constraints that are too rigid, allowing for more flexibility in certain systems where hard constraints might lead to instability or over-constrained systems.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tether_PostSimulation_PositionSmoothing, "Tether.PostSimulation.PositionSmoothing", "Smooths the positions of objects over time, reducing any small positional jitters that might occur due to physics inaccuracies or minor numerical errors.");

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