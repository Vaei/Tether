// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherIntegrationSolver.generated.h"

/**
 * Abstract base class for integration solvers in the Tether physics system.
 * 
 * Integration solvers calculate the physical state of objects over time. They are responsible for
 * integrating velocities and forces to compute new positions and velocities for shapes within the simulation.
 * 
 * This class is intended to be subclassed to implement different integration techniques such as
 * Euler, Runge-Kutta 4th Order, or Verlet integration.
 * 
 * This class is not blueprintable and is meant to be extended through C++.
 */
UCLASS(Abstract, NotBlueprintable)
class TETHERPHYSICS_API UTetherIntegrationSolver : public UObject
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
		float DeltaTime, double WorldTime) const {}
};
