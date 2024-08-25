// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Threading/TetherPhysicsThread.h"

#include "TetherStatics.h"

FTetherPhysicsRunnable* FTetherPhysicsRunnable::Create(UObject* InThreadOwner, float InSimulationFrameRate, EThreadPriority ThreadPriority, uint32 StackSize)
{
	// Don't try to init if owner is being GC'd
	if (!IsValid(InThreadOwner))
	{
		return nullptr;
	}

	// Check if the InThreadOwner implements the ITetherPhysicsThreadInterface
	if (!InThreadOwner->Implements<UTetherPhysicsThreadInterface>())
	{
		const FString ErrorString = FString::Printf(TEXT("[ %s ] { %s } does not implement: %s. Critical Error."),
			*FString(__FUNCTION__), *InThreadOwner->GetName(),
			*UTetherPhysicsThreadInterface::StaticClass()->GetName());

#if WITH_EDITOR
		FMessageLog MsgLog { "PIE" };
		MsgLog.Error(FText::FromString(ErrorString));
#else
		UE_LOG(LogTether, Fatal, TEXT("%s"), *ErrorString);
#endif
		return nullptr;
	}

	// Create and initialize the runnable instance
	FTetherPhysicsRunnable* Runnable = new FTetherPhysicsRunnable(InThreadOwner, InSimulationFrameRate, ThreadPriority, StackSize);
	if (Runnable && Runnable->Init())
	{
		// Successfully created the runnable instance, return it
		return Runnable;
	}
	else
	{
		// Initialization failed, delete the instance
		delete Runnable;
	}

	// Return nullptr if creation failed
	return nullptr;
}

FTetherPhysicsRunnable::FTetherPhysicsRunnable(UObject* InThreadOwner, float InSimulationFrameRate, EThreadPriority ThreadPriority, uint32 StackSize)
	: ThreadOwner(InThreadOwner)
	, Thread(nullptr)
	, bShouldStop(false)
	, PhysicsUpdate(InSimulationFrameRate)
{
	LastStackSize = StackSize;
	ThreadInterface = IsValid(InThreadOwner) ? Cast<ITetherPhysicsThreadInterface>(InThreadOwner) : nullptr;
	if (ThreadInterface)
	{
		StartThread(ThreadPriority);
	}
}

void FTetherPhysicsRunnable::StartThread(EThreadPriority ThreadPriority)
{
	if (!Thread && IsValid(ThreadOwner))
	{
		
		Thread = FRunnableThread::Create(this, TEXT("FTetherPhysicsRunnable"), LastStackSize, ThreadPriority);
	}
}

FTetherPhysicsRunnable::~FTetherPhysicsRunnable()
{
	// Ensure the thread is stopped
	if (Thread)
	{
		Thread->Kill(true);
		delete Thread;
		Thread = nullptr;
	}
}

bool FTetherPhysicsRunnable::Init()
{
	// Initialization logic here
	UE_LOG(LogTether, Log, TEXT("FTetherPhysicsRunnable Initialized"));
	return true;
}

uint32 FTetherPhysicsRunnable::Run()
{
	double LastTime = FPlatformTime::Seconds();
	
	// Main thread loop
	while (!ShouldStop())
	{
		// Get the current time
		double CurrentTime = FPlatformTime::Seconds();
        
		// Calculate DeltaTime
		float DeltaTime = static_cast<float>(CurrentTime - LastTime);
		
		// Update LastTime for the next iteration
		LastTime = CurrentTime;

		// Start the frame with the current DeltaTime
		PhysicsUpdate.StartFrame(DeltaTime);

		// Update at consistent framerate (default 60fps)
		while (PhysicsUpdate.ShouldTick())
		{
			// Physics update logic here
			if (!ThreadInterface->IsPhysicsSimulationPaused())
			{
				ThreadInterface->TickPhysics(PhysicsUpdate.TimeTick);
			}
			PhysicsUpdate.FinalizeTick();
		}

		// Sleep or yield to prevent 100% CPU usage
		FPlatformProcess::Sleep(0.01f);
	}

	return 0;
}

void FTetherPhysicsRunnable::Stop()
{
	// Set the stop flag to true
	FScopeLock Lock(&Mutex);
	bShouldStop = true;
}

void FTetherPhysicsRunnable::Exit()
{
	// Cleanup logic here
	UE_LOG(LogTether, Log, TEXT("FTetherPhysicsRunnable Exiting"));
}

bool FTetherPhysicsRunnable::ShouldStop() const
{
	FScopeLock Lock(&Mutex);
	return bShouldStop || !IsValid(ThreadOwner);
}
