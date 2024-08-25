// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * Handles the timing and updates for physics simulations, ensuring consistent sub-ticks within a frame.
 */
struct FTetherPhysicsUpdate
{
	float TimeTick;
	double RemainingTime;

	FTetherPhysicsUpdate(float SimulationFrameRate)
		: TimeTick(1.f / SimulationFrameRate)
		, RemainingTime(0.f)
	{
	}

	void StartFrame(float DeltaTime)
	{
		// Accumulate the time since the last frame
		RemainingTime += DeltaTime;
	}

	bool ShouldTick() const
	{
		return RemainingTime >= TimeTick;
	}

	void FinalizeTick()
	{
		// Adjust the remaining time for the next sub-tick
		RemainingTime -= TimeTick;
	}
};
