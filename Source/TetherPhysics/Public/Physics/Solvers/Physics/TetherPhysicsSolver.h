// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherPhysicsSolver.generated.h"

/**
 * Abstract base class for physics solvers in the Tether physics system.
 *
 * UTetherPhysicsSolver provides a framework for implementing custom physics solvers that handle
 * various aspects of physical simulations, such as force integration, constraint resolution, or 
 * other custom physics calculations. As an abstract base class, it is designed to be extended
 * to implement specific types of solvers that operate on different types of physical data.
 *
 * The Solve() method is the core of the solver, where the actual physics computations take place.
 * Derived classes should implement this method to define how input data is processed to produce
 * the desired physical outcomes.
 *
 * Key Responsibilities:
 * - Solve: Perform physics calculations using the provided input data and store the results in the output data.
 *
 * Note: This class is abstract and cannot be instantiated directly. It is intended to be subclassed
 * to create specific solvers for different types of physics calculations.
 */
UCLASS(Abstract, NotBlueprintable)
class TETHERPHYSICS_API UTetherPhysicsSolver : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Perform physics calculations based on the input data and produce output results.
	 *
	 * @param InputData  Pointer to the input data containing the current state of the physics objects.
	 * @param OutputData Pointer to the output data where the results of the physics calculations will be stored.
	 * @param Transform  The transformation to be applied during the physics calculations.
	 * @param DeltaTime  The time step for the simulation, used to calculate time-dependent physics effects.
	 */
	virtual void Solve(const FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const {}

	/**
	 * Visualizes the physics solver's key properties for debugging purposes.
	 * 
	 * @param InputData             Pointer to the input data used in the physics solver.
	 * @param OutputData            Pointer to the output data containing the results of the physics calculations.
	 * @param ShapeTransforms       The transform representing the object's position and rotation in the world.
	 * @param PendingDebugText	    Array of Debug Texts that to be drawn by the viewport
	 * @param LifeTime              The duration for which the debug lines should be visible (negative values mean infinite).
	 * @param Proxy					Pointer to the animation instance proxy for drawing debug information.
	 * @param World                 Pointer to the world context in which the debugging visualization occurs.
	 * @param VelocityColor			The color used to visualize the velocity.
	 * @param ForceColor            The color used to visualize the applied forces.
	 * @param AccelerationColor     The color used to visualize the acceleration.
	 * @param bPersistentLines      Whether the debug lines should persist beyond a single frame.
	 * @param Thickness             The thickness of the debug lines.
	 */
	virtual void DrawDebug(const FTetherIO* InputData, FTetherIO* OutputData,
		TMap<const FTetherShape*, const FTransform*> ShapeTransforms,
		TArray<FTetherDebugText>* PendingDebugText = nullptr, float LifeTime = -1.f,
		FAnimInstanceProxy* Proxy = nullptr, const UWorld* World = nullptr,
		const FColor& VelocityColor = FColor::Green, const FColor& ForceColor = FColor::Blue,
		const FColor& AccelerationColor = FColor::Yellow, bool bPersistentLines = false, float Thickness = 1.0f) const {}
};
