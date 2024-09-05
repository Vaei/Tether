// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "TetherPhysicsSolver.h"
#include "TetherPhysicsSolverAngular.generated.h"

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
class TETHERPHYSICS_API UTetherPhysicsSolverAngular : public UTetherPhysicsSolver
{
	GENERATED_BODY()

public:
	/**
	 * Perform angular physics calculations based on the input data and produce output results.
	 *
	 * @param InputData  Pointer to the input data containing the current angular state of the physics objects.
	 * @param OutputData Pointer to the output data where the results of the angular physics calculations will be stored.
	 * @param Transform  The transformation to be applied during the physics calculations.
	 * @param DeltaTime  The time step for the simulation, used to calculate time-dependent angular effects.
	 */
	virtual void Solve(const FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const override;
};
