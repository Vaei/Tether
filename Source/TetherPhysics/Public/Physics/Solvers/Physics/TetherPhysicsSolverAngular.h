// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherPhysicsSolverAngular.generated.h"

struct FTetherDebugText;

/**
 * Physics solver for angular motion in the Tether physics system.
 *
 * UTetherPhysicsSolverAngular is a specialized physics solver that focuses on solving angular motion.
 * It extends the base UTetherPhysicsSolver class and implements the `Solve` method to process
 * angular forces, torques, and other rotational effects within the simulation.
 *
 * This solver is responsible for updating the angular state of objects based on the input data,
 * considering factors such as torque, angular damping, and other rotational dynamics.
 *
 * Key Responsibilities:
 * - Solve: Perform calculations related to angular motion, updating the output data with the results.
 *
 * Note: This class is not intended for use within Blueprints and should be used in C++ to handle
 * angular physics calculations.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherPhysicsSolverAngular : public UObject
{
	GENERATED_BODY()

protected:
	static void ApplyAngularDamping(FVector& AngularVelocity, const FAngularInputSettings& Settings, float DeltaTime);

public:
	/**
	 * Perform angular physics calculations based on the input data and produce output results.
	 *
	 * @param InputData  Pointer to the input data containing the current angular state of the physics objects.
	 * @param OutputData Pointer to the output data where the results of the angular physics calculations will be stored.
	 * @param DeltaTime  The time step for the simulation, used to calculate time-dependent angular effects.
	 * @param WorldTime	 Current WorldTime appended by TimeTicks
	 */
	virtual void Solve(const FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData, float DeltaTime, double WorldTime) const;
	
	/**
	 * Visualizes the physics solver's key properties for debugging purposes.
	 * 
	 * @param InputData             Pointer to the input data used in the physics solver.
	 * @param OutputData            Pointer to the output data containing the results of the physics calculations.
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
	virtual void DrawDebug(const FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData,
		TArray<FTetherDebugText>* PendingDebugText = nullptr, float LifeTime = -1.f,
		FAnimInstanceProxy* Proxy = nullptr, const UWorld* World = nullptr,
		const FColor& VelocityColor = FColor::Emerald, const FColor& ForceColor = FColor::Cyan,
		const FColor& AccelerationColor = FColor::Orange, bool bPersistentLines = false, float Thickness = 1.0f) const;
};
