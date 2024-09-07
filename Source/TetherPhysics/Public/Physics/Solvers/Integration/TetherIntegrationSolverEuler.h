// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIntegrationSolver.h"
#include "TetherIntegrationSolverEuler.generated.h"

/**
 * Euler integration solver for the Tether physics system.
 * 
 * This class implements simple Euler integration, which updates an object's position 
 * and velocity by directly applying the current forces and velocities over a single time step.
 * 
 * Euler integration is computationally cheap but can suffer from stability issues 
 * with large time steps or fast-moving objects.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherIntegrationSolverEuler : public UTetherIntegrationSolver
{
	GENERATED_BODY()

public:
	/**
	 * Perform Euler integration to update positions and rotations.
	 *
	 * This method updates the physical state (position and rotation) of each shape 
	 * in the physics simulation by utilizing the linear and angular velocities 
	 * provided by the linear and angular solvers.
	 * 
	 * @param InputData  Pointer to the input data, containing linear and angular velocities.
	 * @param OutputData Pointer to the output data where the results of the Euler integration will be stored.
	 * @param DeltaTime  The time step for the simulation, used to calculate the new state.
	 */
	virtual void Solve(const FTetherIO* InputData, FTetherIO* OutputData, float DeltaTime) const override;
};
