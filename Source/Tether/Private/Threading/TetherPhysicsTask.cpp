// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Threading/TetherPhysicsTask.h"


void FTetherPhysicsTask::DoWork() const
{
	if (ThreadInterface)
	{
		ThreadInterface->TickPhysics(DeltaTime);
	}
}
