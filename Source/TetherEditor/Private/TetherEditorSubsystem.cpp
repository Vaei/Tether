// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherEditorSubsystem.h"

#include "TetherDataAsset.h"
#include "TetherSettings.h"
#include "TetherEditorShapeActor.h"
#include "Physics/Collision/TetherCollisionDetectionBroadPhase.h"
#include "Physics/Hashing/TetherHashingSpatial.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverAngular.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverLinear.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherEditorSubsystem)

namespace FTether
{
	TAutoConsoleVariable<bool> CVarTetherMatchFramerateToSimRate(TEXT("p.Tether.MatchFramerateToSimRate"), true, TEXT("Set t.maxfps=SimulationFrameRate on BeginPlay so the render tick runs at the same rate as Tether, if it can manage to"));
}

void UTetherEditorSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const TSoftObjectPtr<UTetherDataAsset>& SoftData = UTetherSettings::Get()->EditorSubsystemDataAsset;
	if (!SoftData.IsNull())
	{
		DataAsset = SoftData.LoadSynchronous();
	}

	if (!ensureAlways(DataAsset))
	{
		// Can never run
		return;
	}

	bHasWorldBegunPlay = true;

	// Grab solvers from the data asset
	SharedData.Solvers = FTetherCommonSharedSolvers {
		DataAsset->HashingSystem, DataAsset->CollisionDetectionHandler, DataAsset->BroadPhaseCollisionDetection,
		DataAsset->NarrowPhaseCollisionDetection, DataAsset->ContactSolver
	};
	
	SharedData.SpatialHashingInput = DataAsset->SpatialHashingInput;

	PhysicsUpdate = { DataAsset->SimulationFrameRate };

	// Match render tick to physics tick, if the engine can keep up
	if (FTether::CVarTetherMatchFramerateToSimRate.GetValueOnGameThread())
	{
		if (GEngine)
		{
			// GEngine->SetMaxFPS(SimulationFrameRate);  // We don't use this, because we can't then clear it easily
	
			FString Command = FString::Printf(TEXT("t.maxfps %f"), DataAsset->SimulationFrameRate);
			GEngine->Exec(nullptr, *Command);
		}
	}
	
	// Reset potentially unique logging
	MessageLog.ResetMessageLogs();

	// Register debug drawing
	DebugTextService.Initialize(this);
}

void UTetherEditorSubsystem::Deinitialize()
{
	DebugTextService.Deinitialize();
	
	Super::Deinitialize();
}

bool UTetherEditorSubsystem::UpdateGameplayTagReferences()
{
	SharedData.Solvers.UpdateSolverData(DataAsset->HashingSystem, DataAsset->CollisionDetectionHandler,
		DataAsset->BroadPhaseCollisionDetection, DataAsset->NarrowPhaseCollisionDetection, DataAsset->ContactSolver);

	if (!ensure(SharedData.Solvers.CurrentCollisionDetectionHandler))
	{
		return false;
	}
	
	return true;
}

void UTetherEditorSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasWorldBegunPlay || ShapeActors.Num() == 0)
	{
		return;
	}

	// Detect gameplay tag changes and grab the newly referenced object
	if (!UpdateGameplayTagReferences())
	{
		return;
	}
	
	// Build Arrays, Maps, Input data for each shape
	TArray<FTetherShape*> Shapes;	// Shapes grabbed from each editor shape actor
	TArray<FVector> Origins;		// Averaged locations of all editor shape actors
	TMap<const FTetherShape*, ATetherEditorShapeActor*> ShapeActorMap;
	for (ATetherEditorShapeActor* Actor : ShapeActors)
	{
		// Grab Shape
		FTetherShape* Shape = Actor->GetTetherShape();

		// Convert to world space, initializing from the actor's transform
		if (!Shape->IsWorldSpace())
		{
			Shape->ToWorldSpace(Actor->GetActorTransform());
		}

		// Add to Shapes Array
		Shapes.Add(Shape);

		// Cache and update solvers
		FTetherCommonShapeData* const& SData = ShapeData.FindOrAdd(Shape, &Actor->ShapeData);
		SData->InitializeShapeData();
		SData->Solvers.UpdateSolvers();
		
		// Cache transform for computing origin
		Origins.Add(Shape->GetAppliedWorldTransform().GetLocation());
		
		// Add to ShapeActorMap TMap
		ShapeActorMap.Add(Shape, Actor);
	}

	// Initialize shared data
	SharedData.InitializeSharedData();

	// Compute an origin at the center of all shape actors
	FVector OriginPoint = FVector::ZeroVector;

	for (const FVector& Origin : Origins)
	{
		OriginPoint += Origin;
	}
	OriginPoint /= Origins.Num();
	FTransform Origin = { FQuat::Identity, OriginPoint };

	double WorldTime = GetWorld()->GetTimeSeconds();
	
	// Start the frame with the current DeltaTime
	PhysicsUpdate.StartFrame(DeltaTime);

	// Update at consistent framerate (default 60fps)
	while (PhysicsUpdate.ShouldTick())
	{
		const float& TimeTick = PhysicsUpdate.TimeTick;

		/* Spatial Hashing - Generate shape pairs based on proximity and efficiency ratings for priority */
		if (SharedData.Solvers.CurrentHashingSystem)
		{
			SharedData.Solvers.CurrentHashingSystem->Solve(&Shapes, &SharedData.SpatialHashingInput,
				&SharedData.SpatialHashingOutput, Origin, TimeTick, WorldTime);
			
			SharedData.Solvers.CurrentHashingSystem->DrawDebug(&Shapes, &SharedData.SpatialHashingInput,
				&SharedData.SpatialHashingOutput, Origin, &DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
		}
		
		/* Solve Broad-Phase Collision */
		if (SharedData.Solvers.CurrentBroadPhaseCollisionDetection)
		{
			// Common optimization step where you quickly check if objects are close enough to potentially collide.
			// It reduces the number of detailed collision checks needed in the narrow phase.
			
			SharedData.Solvers.CurrentBroadPhaseCollisionDetection->DetectCollision(&SharedData.BroadPhaseInput,
				&SharedData.BroadPhaseOutput, SharedData.Solvers.CurrentCollisionDetectionHandler, TimeTick, WorldTime);
			
			SharedData.Solvers.CurrentBroadPhaseCollisionDetection->DrawDebug(&Shapes, &SharedData.BroadPhaseInput,
				&SharedData.BroadPhaseOutput, &DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
		}

		// Update in preparation for Narrow-Phase
		SharedData.NarrowPhaseInput.LinearOutputs.Reset();
		SharedData.NarrowPhaseInput.AngularOutputs.Reset();

		// Execute per-shape solvers
		for (auto& ShapeItr : ShapeData)
		{
			FTetherShape* Shape = ShapeItr.Key;
			FTetherCommonShapeData* Data = ShapeItr.Value;

			/* Pre-Solve Activity State (Wake) */
			
			if (Data->Solvers.CurrentActivityStateHandler)
			{
				Data->Solvers.CurrentActivityStateHandler->PreSolveWake(Shape, &Data->ActivityInput,
					&Data->LinearInput, &Data->AngularInput, TimeTick, WorldTime);
			}
			
			/* Solve Linear & Angular Physics
			 *	These steps calculate the velocities that will be applied to the object. By solving linear and angular
			 *	physics first, you get the raw velocities that are then used in integration. */

			if (Data->Solvers.CurrentLinearSolver)
			{
				Data->Solvers.CurrentLinearSolver->Solve(Shape, &Data->LinearInput, &Data->LinearOutput,
					TimeTick, WorldTime);
				
				Data->Solvers.CurrentLinearSolver->DrawDebug(Shape, &Data->LinearInput, &Data->LinearOutput,
					&DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
			}

			if (Data->Solvers.CurrentAngularSolver)
			{
				Data->Solvers.CurrentAngularSolver->Solve(Shape, &Data->AngularInput, &Data->AngularOutput,
					TimeTick, WorldTime);
				
				Data->Solvers.CurrentAngularSolver->DrawDebug(Shape, &Data->AngularInput, &Data->AngularOutput,
					&DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
			}

			/* Post-Solve Activity State (Sleep) */
			if (Data->Solvers.CurrentActivityStateHandler)
			{
				Data->Solvers.CurrentActivityStateHandler->PostSolveSleep(Shape, &Data->ActivityInput,
					&Data->LinearInput, &Data->AngularInput, &Data->LinearOutput,
					&Data->AngularOutput, TimeTick, WorldTime);
				
				Data->Solvers.CurrentActivityStateHandler->DrawDebug(Shape, &Data->ActivityInput,
					&DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
			}

			/* Solve Integration
			 *	This part of the solver takes the results from the linear and angular solvers and updates the position
			 *	and orientation of objects over time. It essentially integrates the calculated forces and torques to
			 *	determine how an object should move in the next time step.
			 */
			if (Data->Solvers.CurrentIntegrationSolver)
			{
				Data->Solvers.CurrentIntegrationSolver->Solve(Shape, &Data->IntegrationInput, &Data->IntegrationOutput,
					TimeTick, WorldTime);

				// Update shape with new transform
				Shape->ToWorldSpace(Data->IntegrationOutput.Transform);
			}

			// @TODO Test this and find a proper use-case
			// // Record state of all objects post-integration for replay purposes
			// if (Data->Solvers.CurrentReplaySystem)
			// {
			// 	Data->Solvers.CurrentReplaySystem->RecordPhysicsState(Shape, &Data->RecordedData, WorldTime,
			// 		&Data->LinearInput, &Data->AngularInput);
			// }

			// Update in preparation for Narrow-Phase
			SharedData.NarrowPhaseInput.LinearOutputs.Add(Shape, &Data->LinearOutput);
			SharedData.NarrowPhaseInput.AngularOutputs.Add(Shape, &Data->AngularOutput);
		}
		// ~Execute per-shape solvers

		// @todo Solve Narrow-Phase Collision
		if (SharedData.Solvers.CurrentNarrowPhaseCollisionDetection)
		{
			SharedData.Solvers.CurrentNarrowPhaseCollisionDetection->DetectCollision(&SharedData.NarrowPhaseInput,
				&SharedData.NarrowPhaseOutput, SharedData.Solvers.CurrentCollisionDetectionHandler, TimeTick, WorldTime);
			
			SharedData.Solvers.CurrentNarrowPhaseCollisionDetection->DrawDebug(&Shapes,
				&SharedData.NarrowPhaseInput, &SharedData.NarrowPhaseOutput,
				&DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
		}

		// This step checks for actual collisions using detailed geometry after the object has been moved.
		// It’s a more precise and computationally expensive check compared to the broad phase.

		// @todo Solve Contact

		// After detecting a collision, this step resolves it by adjusting the object's position and velocities. It
		// prevents interpenetration and handles the physical response of the objects involved in the collision.

		// Each shape has its own contact solver assigned
		// e.g. Soft body, Rigid body

		// @todo Solve Constraints (Multiple!)

		// This handles constraints that limit or define the relationships between objects, such as joints
		// (e.g., hinges or sliders) that allow or restrict certain movements between connected objects.
		
		// Constraints are applied last because they often need to override other physical behaviors. For example,
		// if accessory_01 is constrained to follow spine_01, the constraint solver will ensure this attachment is
		// respected, regardless of the results of other physics calculations. You might have multiple constraints to
		// solve, depending on the complexity of your simulation.

		// @todo Solve Post-Projection

		// Post-projection usually comes after both contact and constraint solvers. The reason is that both contact
		// resolution and constraints can introduce small positional errors due to floating-point precision,
		// integration errors, or other numerical artifacts. Post-projection corrects these errors by moving objects
		// directly into a valid state after both the contact and constraint solvers have run.
		
		WorldTime += TimeTick;
		PhysicsUpdate.FinalizeTick();
	}

	// Delta can be incredibly low, especially on first run, so that it never enters the while() loop
	// Alternatively, we could do while() instead, but that means inconsistent ticks when the render delta is tiny
	if (!PhysicsUpdate.bEverTicked)
	{
		return;
	}
	
	// Update actual transforms based on integration result
	for (const auto& ActorItr : ShapeActorMap)
	{
		const FTetherShape* const& Shape = ActorItr.Key;
		ATetherEditorShapeActor* Actor = ActorItr.Value;
		if (ensure(IsValid(Actor)))
		{
			const FTransform& Transform = Shape->GetAppliedWorldTransform();
			Actor->SetActorTransform(Transform);
		}
	}

	// Print any pending messages
	MessageLog.ProcessMessages();
}

TStatId UTetherEditorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTetherEditorSubsystem, STATGROUP_Tickables);
}