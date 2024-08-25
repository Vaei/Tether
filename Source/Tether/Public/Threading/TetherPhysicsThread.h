// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ITetherPhysicsThreadInterface.h"
#include "TetherPhysicsUpdate.h"

/**
 * Separate physics thread to optionally use
 * Improves determinism of results by updating in a single thread away from the game thread for greater consistency
 *
 * Defaults to TPri_BelowNormal helps ensure that your physics thread doesn't consume too much CPU at the expense of
 * the main game thread. The game thread is typically more critical since it handles rendering, input, and other
 * essential tasks. By giving your physics thread a lower priority, you allow the main thread to have preferential
 * access to the CPU. If your physics sim is more critical, consider increasing it.
 *
 * Owner must implement ITetherPhysicsThreadInterface
 */
class TETHER_API FTetherPhysicsRunnable : public FRunnable
{
public:
	/**
	 * Static factory method to create an instance of FTetherPhysicsRunnable.
	 * 
	 * @param InThreadOwner The owner of the thread that implements ITetherPhysicsThreadInterface.
	 * @param InSimulationFrameRate The framerate to simulate at, can result in multiple updates per tick
	 * @param ThreadPriority The priority of the thread.
	 * @param StackSize How much stack memory to allocate for the thread. At 0 the platform's default stack size will be used.
	 * @return A new instance of FTetherPhysicsRunnable if creation was successful, otherwise nullptr.
	 */
	static FTetherPhysicsRunnable* Create(UObject* InThreadOwner = nullptr, float InSimulationFrameRate = 60.f, EThreadPriority ThreadPriority = TPri_BelowNormal, uint32 StackSize = 0);

	/** Starts the thread if in SingleThread mode */
	void StartThread(EThreadPriority ThreadPriority);
	
	// Begin FRunnable interface.
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	// ~End FRunnable interface.

	/** Thread-safe way to check if the thread should stop */
	bool ShouldStop() const;

protected:
	FTetherPhysicsRunnable(UObject* InThreadOwner = nullptr, float InSimulationFrameRate = 60.f, EThreadPriority ThreadPriority = TPri_BelowNormal, uint32 StackSize = 0);
	virtual ~FTetherPhysicsRunnable() override;

	UObject* ThreadOwner;
	ITetherPhysicsThreadInterface* ThreadInterface;

	/** The thread handle */
	FRunnableThread* Thread;

	/** Sync object to prevent data race conditions */
	mutable FCriticalSection Mutex;

	/** Whether the thread should stop */
	bool bShouldStop;

	FTetherPhysicsUpdate PhysicsUpdate;

	uint32 LastStackSize;
};
