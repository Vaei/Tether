// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherPhysicsTypes.h"

#include "TetherSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherPhysicsTypes)

void FTetherCommonSharedSolverData::UpdateSolverData(const FGameplayTag& HashingSystem,
	const FGameplayTag& CollisionDetectionHandler, const FGameplayTag& BroadPhaseCollisionDetection,
	const FGameplayTag& NarrowPhaseCollisionDetection, const FGameplayTag& ContactSolver)
{
	if (LastHashingSystem != HashingSystem)
	{
		LastHashingSystem = HashingSystem;
		CurrentHashingSystem = UTetherSettings::GetHashingSystem(HashingSystem);
	}
	
	if (LastCollisionDetectionHandler != CollisionDetectionHandler)
	{
		LastCollisionDetectionHandler = CollisionDetectionHandler;
		CurrentCollisionDetectionHandler = UTetherSettings::GetCollisionDetectionHandler(CollisionDetectionHandler);
	}

	if (LastBroadPhaseCollisionDetection != BroadPhaseCollisionDetection)
	{
		LastBroadPhaseCollisionDetection = BroadPhaseCollisionDetection;
		CurrentBroadPhaseCollisionDetection = UTetherSettings::GetBroadPhaseSystem(BroadPhaseCollisionDetection);
	}

	if (LastNarrowPhaseCollisionDetection != NarrowPhaseCollisionDetection)
	{
		LastNarrowPhaseCollisionDetection = NarrowPhaseCollisionDetection;
		CurrentNarrowPhaseCollisionDetection = UTetherSettings::GetNarrowPhaseSystem(NarrowPhaseCollisionDetection);
	}

	if (LastContactSolver != ContactSolver)
	{
		LastContactSolver = ContactSolver;
		// CurrentContactSolver = UTetherSettings::GetNarrowPhaseSystem(ContactSolver);
	}
}

void FTetherCommonShapeSolverData::UpdateSolverData(const FGameplayTag& ActivityStateHandler,
	const FGameplayTag& LinearSolver, const FGameplayTag& AngularSolver, const FGameplayTag& IntegrationSolver,
	const FGameplayTag& ReplaySystem)
{
	// Detect any change in the gameplay tag, and update with the corresponding object
	
	if (LastActivityStateHandler != ActivityStateHandler)
	{
		LastActivityStateHandler = ActivityStateHandler;
		CurrentActivityStateHandler = UTetherSettings::GetActivityStateHandler<UTetherActivityStateHandler>(ActivityStateHandler);
	}

	if (LastLinearSolver != LinearSolver)
	{
		LastLinearSolver = LinearSolver;
		CurrentLinearSolver = UTetherSettings::GetLinearPhysicsSolver<UTetherPhysicsSolverLinear>(LinearSolver);
	}

	if (LastAngularSolver != AngularSolver)
	{
		LastAngularSolver = AngularSolver;
		CurrentAngularSolver = UTetherSettings::GetAngularPhysicsSolver<UTetherPhysicsSolverAngular>(AngularSolver);
	}

	if (LastIntegrationSolver != IntegrationSolver)
	{
		LastIntegrationSolver = IntegrationSolver;
		CurrentIntegrationSolver = UTetherSettings::GetIntegrationSolver<UTetherIntegrationSolver>(IntegrationSolver);
	}
	
	if (LastReplaySystem != ReplaySystem)
	{
		LastReplaySystem = ReplaySystem;
		CurrentReplaySystem = UTetherSettings::GetReplaySystem(ReplaySystem);
	}
}

void FTetherCommonShapeSolvers::UpdateSolvers()
{
	UpdateSolverData(ActivityStateHandler, LinearSolver, AngularSolver, IntegrationSolver, ReplaySystem);
}