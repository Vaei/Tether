// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherIntegrationSolver.generated.h"

/**
 * Abstract base class for integration solvers in the Tether physics system.
 * This class is responsible for calculating the physical state of objects over time.
 * Integration solvers compute new positions and velocities by integrating forces and velocities.
 * 
 * This class is intended to be subclassed for implementing different numerical integration techniques
 * such as Euler, Runge-Kutta, or Verlet integration.
 * 
 * This class is not blueprintable and should be extended via C++.
 */
UCLASS(Abstract, NotBlueprintable)
class TETHERPHYSICS_API UTetherIntegrationSolver : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Perform physics integration to update positions and velocities.
	 *
	 * @param InputData  Pointer to the input data for the current physics state.
	 * @param OutputData Pointer to the output data storing the results of the physics update.
	 * @param DeltaTime  The time step used for time-dependent calculations.
	 * @param WorldTime	 Current WorldTime appended by TimeTicks
	 */
	virtual void Solve(const FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData,
		float DeltaTime, double WorldTime) const {}
};
