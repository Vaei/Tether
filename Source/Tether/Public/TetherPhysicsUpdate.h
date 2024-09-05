// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Handles the timing and updates for physics simulations, ensuring consistent sub-ticks within a frame.
 *
 * FTetherPhysicsUpdate manages the subdivision of time within a physics simulation frame, allowing for multiple
 * updates (sub-ticks) within a single frame if necessary. This ensures that the physics simulation remains stable
 * and consistent, even when frame rates vary.
 *
 * Example Usage:
 * 
 * void AMyActor::Tick(float DeltaTime)
 * {
 *     // Start the frame with the current DeltaTime
 *     PhysicsUpdate.StartFrame(DeltaTime);
 *
 *     // Update at a consistent framerate (e.g., 60fps)
 *     while (PhysicsUpdate.ShouldTick())
 *     {
 *         // Perform physics update logic here, using the fixed time step
 *         TickPhysics(PhysicsUpdate.TimeTick);
 *
 *         // Finalize the tick, preparing for the next potential sub-tick
 *         PhysicsUpdate.FinalizeTick();
 *     }
 * }
 */
struct TETHER_API FTetherPhysicsUpdate
{
	/** The fixed time step for each sub-tick, based on the desired simulation frame rate */
	float TimeTick;

	/** The accumulated time since the last sub-tick, used to determine if a new sub-tick is needed */
	float RemainingTime;

	/**
	 * Initialize the time tick and remaining time based on the simulation frame rate.
	 * 
	 * @param SimulationFrameRate The desired number of physics ticks per second.
	 */
	FTetherPhysicsUpdate(float SimulationFrameRate)
		: TimeTick(1.f / SimulationFrameRate)	// Calculate the time per tick
		, RemainingTime(0.f)					// Initialize the remaining time to zero
	{}

	/**
	 * Starts the frame by accumulating the time since the last frame.
	 * 
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	void StartFrame(float DeltaTime)
	{
		// Accumulate the time since the last frame
		RemainingTime += DeltaTime;
	}

	/**
	 * Determines whether the physics simulation should perform a sub-tick.
	 * 
	 * @return True if the accumulated time is greater than or equal to the time tick, indicating that a sub-tick should occur.
	 */
	bool ShouldTick() const
	{
		return RemainingTime >= TimeTick;
	}

	/**
	 * Finalizes the sub-tick by adjusting the remaining time for the next sub-tick.
	 */
	void FinalizeTick()
	{
		// Adjust the remaining time for the next sub-tick
		RemainingTime -= TimeTick;
	}
};
