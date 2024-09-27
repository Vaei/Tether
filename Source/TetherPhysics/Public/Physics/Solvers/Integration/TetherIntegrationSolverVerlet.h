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
	 * Perform Verlet integration to update positions and rotations.
	 *
	 * This method updates the physical state (position and rotation) of each shape 
	 * in the physics simulation by utilizing the linear velocity and angular velocity 
	 * provided by the linear and angular solvers, respectively. 
	 * 
	 * @param InputData  Pointer to the input data, containing linear and angular velocities.
	 * @param OutputData Pointer to the output data where the results of the Verlet integration will be stored.
	 * @param DeltaTime  The time step for the simulation, used to calculate the new state.
	 */
	virtual void Solve(const FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData,
		float DeltaTime, double WorldTime) const override;
};
