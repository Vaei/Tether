// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ITetherPhysicsThreadInterface.h"

class FTetherPhysicsTask : public FNonAbandonableTask
{
public:
	FTetherPhysicsTask(ITetherPhysicsThreadInterface* InThreadInterface, float InDeltaTime)
		: ThreadInterface(InThreadInterface), DeltaTime(InDeltaTime)
	{}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FPhysicsTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	void DoWork() const
	{
		
	}

private:
	ITetherPhysicsThreadInterface* ThreadInterface;
	float DeltaTime;
};
