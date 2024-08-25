// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "Misc/ScopeLock.h"

class FTetherPhysicsRunnable : public FRunnable
{
public:
	FTetherPhysicsRunnable();
	virtual ~FTetherPhysicsRunnable();

	// Begin FRunnable interface.
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	// End FRunnable interface.

	// Thread-safe way to check if the thread should stop
	bool ShouldStop() const;

private:
	// The thread handle
	FRunnableThread* Thread;

	// Sync object to prevent data race conditions
	mutable FCriticalSection Mutex;

	// Whether the thread should stop
	bool bShouldStop;
};

FTetherPhysicsRunnable::FTetherPhysicsRunnable()
	: Thread(nullptr)
	, bShouldStop(false)
{
	Thread = FRunnableThread::Create(this, TEXT("FTetherPhysicsRunnable"), 0, TPri_BelowNormal);
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
	UE_LOG(LogTemp, Log, TEXT("FTetherPhysicsRunnable Initialized"));
	return true;
}

uint32 FTetherPhysicsRunnable::Run()
{
	// Main thread loop
	while (!ShouldStop())
	{
		// Physics update logic here

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
	UE_LOG(LogTemp, Log, TEXT("FTetherPhysicsRunnable Exiting"));
}

bool FTetherPhysicsRunnable::ShouldStop() const
{
	FScopeLock Lock(&Mutex);
	return bShouldStop;
}
