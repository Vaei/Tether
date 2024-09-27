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
	 * Perform physics integration to update positions and velocities.
	 * 
	 * This method is called for each shape in the simulation to compute new physical states 
	 * based on the input data and the current state of the world. The output data contains 
	 * the updated physical state after the integration step.
	 *
	 * @param Shape		The shape being simulated.
	 * @param InputData  Pointer to the input data for the current physics state.
	 * @param OutputData Pointer to the output data storing the results of the physics update.
	 * @param DeltaTime  The time step for the simulation, used to compute time-dependent changes.
	 * @param WorldTime  The current world time appended by TimeTicks.
	 */
	virtual void Solve(const FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData,
		float DeltaTime, double WorldTime) const override;
};
