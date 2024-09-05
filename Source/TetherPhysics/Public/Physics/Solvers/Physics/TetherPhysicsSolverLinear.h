// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "TetherPhysicsSolver.h"
#include "TetherPhysicsSolverLinear.generated.h"

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
class TETHERPHYSICS_API UTetherPhysicsSolverLinear : public UTetherPhysicsSolver
{
	GENERATED_BODY()

public:
	/**
	 * Perform linear physics calculations based on the input data and produce output results.
	 *
	 * @param InputData  Pointer to the input data containing the current linear state of the physics objects.
	 * @param OutputData Pointer to the output data where the results of the linear physics calculations will be stored.
	 * @param Transform  The transformation to be applied during the physics calculations.
	 * @param DeltaTime  The time step for the simulation, used to calculate time-dependent linear effects.
	 */
	virtual void Solve(const FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const override;
};
