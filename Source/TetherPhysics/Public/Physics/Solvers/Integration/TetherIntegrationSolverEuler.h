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
