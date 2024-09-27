// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherPhysicsSolverLinear.generated.h"

struct FTetherDebugText;

/**
 * Physics solver for linear motion in the Tether physics system.
 *
 * UTetherPhysicsSolverLinear is a specialized physics solver that focuses on solving linear motion.
 * It extends the base UTetherPhysicsSolver class and implements the `Solve` method to process
 * linear forces, velocities, and other translational effects within the simulation.
 *
 * This solver is responsible for updating the linear state of objects based on the input data,
 * considering factors such as applied forces, linear damping, and other translational dynamics.
 *
 * Key Responsibilities:
 * - Solve: Perform calculations related to linear motion, updating the output data with the results.
 *
 * Note: This class is not intended for use within Blueprints and should be used in C++ to handle
 * linear physics calculations.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherPhysicsSolverLinear : public UObject
{
	GENERATED_BODY()

protected:
	/**
	 * Applies linear damping to the velocity of the shape, reducing its linear speed over time based on the damping model.
	 *
	 * @param Velocity   The current velocity of the shape, modified in place.
	 * @param Settings   Input settings that contain damping parameters.
	 * @param DeltaTime  The time step used for applying the damping effect.
	 */
	static void ApplyLinearDamping(FVector& Velocity, const FLinearInputSettings& Settings, float DeltaTime);
	
public:
	/**
	 * Perform linear physics calculations for the given shape, updating its velocity and position.
	 *
	 * This method solves linear forces, acceleration, and damping for a specific shape, using the input data
	 * to compute the updated velocity and other linear properties.
	 *
	 * @param Shape      The shape whose linear motion is being solved.
	 * @param InputData  Pointer to the input data containing the current state of the shape's linear physics.
	 * @param OutputData Pointer to the output data where the results of the linear physics calculations will be stored.
	 * @param DeltaTime  The time step for the simulation, used to calculate time-dependent linear effects.
	 * @param WorldTime  The current world time, typically used for logging or time-sensitive calculations.
	 */
	virtual void Solve(FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData, float DeltaTime,
		float WorldTime) const;

	/**
	 * Visualizes the linear motion solver's key properties for debugging purposes.
	 * 
	 * This function draws debug visualizations for velocity, forces, and acceleration based on the current
	 * state of the shape's linear motion, helping to visualize the solver's impact on each shape.
	 * 
	 * @param Shape                 The shape for which the debug information is being drawn.
	 * @param InputData             Pointer to the input data used in the linear physics solver.
	 * @param OutputData            Pointer to the output data containing the results of the physics calculations.
	 * @param PendingDebugText	    Array of Debug Texts that will be drawn by the viewport.
	 * @param LifeTime              The duration for which the debug lines should be visible (negative values mean infinite).
	 * @param Proxy					Pointer to the animation instance proxy for drawing debug information (optional).
	 * @param World                 Pointer to the world context in which the debugging visualization occurs (optional).
	 * @param VelocityColor			The color used to visualize the velocity.
	 * @param ForceColor            The color used to visualize the applied forces.
	 * @param AccelerationColor     The color used to visualize the acceleration.
	 * @param bPersistentLines      Whether the debug lines should persist beyond a single frame.
	 * @param Thickness             The thickness of the debug lines.
	 */
	virtual void DrawDebug(const FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData,
		TArray<FTetherDebugText>* PendingDebugText = nullptr, float LifeTime = -1.f,
		FAnimInstanceProxy* Proxy = nullptr, const UWorld* World = nullptr,
		const FColor& VelocityColor = FColor::Green, const FColor& ForceColor = FColor::Blue,
		const FColor& AccelerationColor = FColor::Yellow, bool bPersistentLines = false, float Thickness = 1.0f) const;
};
