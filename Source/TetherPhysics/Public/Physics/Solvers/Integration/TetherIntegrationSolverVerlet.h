// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIntegrationSolver.h"
#include "TetherIntegrationSolverVerlet.generated.h"

/**
 * Verlet integration solver for the Tether physics system.
 * 
 * This class implements Verlet integration, which calculates an object's new position based on 
 * both its current and previous positions, bypassing explicit velocity calculations.
 * 
 * Verlet integration is particularly stable for systems involving constraints (e.g., springs) 
 * and tends to handle high velocities and large time steps better than Euler integration.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherIntegrationSolverVerlet : public UTetherIntegrationSolver
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
