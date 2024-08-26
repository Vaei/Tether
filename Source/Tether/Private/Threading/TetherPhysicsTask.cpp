// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Threading/TetherPhysicsTask.h"

#include "TetherActorComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherPhysicsTask)

UTetherPhysicsTask::UTetherPhysicsTask(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, LinearSolverTag(FGameplayTag::EmptyTag)
	, AngularSolverTag(FGameplayTag::EmptyTag)
{
	bTickingTask = true;
}

UTetherPhysicsTask* UTetherPhysicsTask::TetherPhysicsTask(UTetherActorComponent* InTaskOwner, FGameplayTag LinearSolver, FGameplayTag AngularSolver, uint8 TaskPriority)
{
	if (!IsValid(InTaskOwner))
	{
		return nullptr;
	}

	check(InTaskOwner->Implements<UGameplayTaskOwnerInterface>());

	if (UTetherPhysicsTask* Task = NewObject<UTetherPhysicsTask>(GetTransientPackage(), StaticClass()))
	{
		IGameplayTaskOwnerInterface* OwnerInterface = CastChecked<IGameplayTaskOwnerInterface>(InTaskOwner);
		Task->InitTask(*OwnerInterface, TaskPriority);
		
		Task->LinearSolverTag = LinearSolver;
		Task->AngularSolverTag = AngularSolver;
		
		return Task;
	}

	return nullptr;
}

void UTetherPhysicsTask::SetLinearSolver(FGameplayTag LinearSolver)
{
}

void UTetherPhysicsTask::SetAngularSolver(FGameplayTag AngularSolver)
{
}
