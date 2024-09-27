// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIntegrationSolver.h"
#include "TetherIntegrationSolverRK4.generated.h"

/**
 * RK4 (Runge-Kutta 4th order) integration solver for the Tether physics system.
 * 
 * This class updates the positions and rotations of simulated objects using RK4 integration,
 * which offers higher accuracy compared to Euler or Verlet methods by sampling the motion
 * at multiple points within the time step.
 *
 * RK4 is more computationally expensive but provides greater stability, especially in simulations
 * with varying forces or high velocities.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherIntegrationSolverRK4 : public UTetherIntegrationSolver
{
	GENERATED_BODY()

public:
	/**
	 * Perform RK4 integration to update positions and rotations.
	 *
	 * This method updates the physical state (position and rotation) of each shape 
	 * in the physics simulation by utilizing the linear and angular velocities 
	 * provided by the linear and angular solvers.
	 * 
	 * @param InputData  Pointer to the input data, containing linear and angular velocities.
	 * @param OutputData Pointer to the output data where the results of the RK4 integration will be stored.
	 * @param DeltaTime  The time step for the simulation, used to calculate the new state.
	 */
	virtual void Solve(const FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData,
		float DeltaTime, double WorldTime) const override;
};
