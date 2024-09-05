// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherEditorSubsystem.h"

#include "TetherSettings.h"
#include "TetherEditorShapeActor.h"
#include "Physics/Collision/TetherCollisionDetectionBroadPhase.h"
#include "Physics/Hashing/TetherHashingSpatial.h"
#include "Physics/Replay/TetherReplay.h"
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

	bHasWorldBegunPlay = true;

	PhysicsUpdate = { SimulationFrameRate };

	// Match render tick to physics tick, if the engine can keep up
	if (FTether::CVarTetherMatchFramerateToSimRate.GetValueOnGameThread())
	{
		if (GEngine)
		{
			// GEngine->SetMaxFPS(SimulationFrameRate);  // We don't use this, because we can't then clear it easily
	
			FString Command = FString::Printf(TEXT("t.maxfps %f"), SimulationFrameRate);
			GEngine->Exec(nullptr, *Command);
		}
	}

	SpatialHashingInput.BucketSizeMode = ETetherBucketSizingStrategy::Automatic;
}

void UTetherEditorSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasWorldBegunPlay || ShapeActors.Num() == 0)
	{
		return;
	}

	// Compute an origin at the center of all shape actors
	FVector OriginPoint = FVector::ZeroVector;
	TArray<FVector> Origins;
	Algo::Transform(ShapeActors, Origins, [](const ATetherEditorShapeActor* Actor)
	{
		return Actor->GetActorLocation();
	});
	
	for (const FVector& Origin : Origins)
	{
		OriginPoint += Origin;
	}
	OriginPoint /= Origins.Num();
	FTransform Origin = { FQuat::Identity, OriginPoint };
	
	// Grab all shapes from each actor
	TArray<FTetherShape*> Shapes;
	Algo::Transform(ShapeActors, Shapes, [](ATetherEditorShapeActor* Actor)
	{
		FTetherShape* Shape = Actor->GetTetherShape();
		Shape->ToWorldSpace(Actor->GetActorTransform());
		return Shape;
	});

	// Pass shapes to Inputs
	SpatialHashingInput.Shapes = &Shapes;
	BroadPhaseInput.Shapes = &Shapes;

	// Detect gameplay tag changes and grab the newly referenced object
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

	if (LastLinearSolver != LinearSolver)
	{
		LastLinearSolver = LinearSolver;
		CurrentLinearSolver = UTetherSettings::GetSolver<UTetherPhysicsSolverLinear>(LinearSolver);
	}

	if (LastAngularSolver != AngularSolver)
	{
		LastAngularSolver = AngularSolver;
		CurrentAngularSolver = UTetherSettings::GetSolver<UTetherPhysicsSolverAngular>(AngularSolver);
	}

	if (LastReplaySystem != ReplaySystem)
	{
		LastReplaySystem = ReplaySystem;
		CurrentReplaySystem = UTetherSettings::GetReplaySystem(ReplaySystem);
	}

	if (!ensure(CurrentCollisionDetectionHandler))
	{
		return;
	}
	
	// Start the frame with the current DeltaTime
	PhysicsUpdate.StartFrame(DeltaTime);

	// Update at consistent framerate (default 60fps)
	while (PhysicsUpdate.ShouldTick())
	{
		const float& TimeTick = PhysicsUpdate.TimeTick;
		
		// 0. Spatial Hashing - Generate shape pairs based on proximity and efficiency ratings for priority
		if (CurrentHashingSystem)
		{
			CurrentHashingSystem->Solve(&SpatialHashingInput, &SpatialHashingOutput, Origin, TimeTick);
			CurrentHashingSystem->DrawDebug(&SpatialHashingInput, &SpatialHashingOutput, Origin, nullptr, GetWorld());
		}
		
		// 1. Solve Broad-Phase Collision
		if (CurrentBroadPhaseCollisionDetection)
		{
			// Optional but common optimization step where you quickly check if objects are close enough to potentially
			// collide. It reduces the number of detailed collision checks needed in the narrow phase.
			BroadPhaseInput.Shapes = &Shapes;
			BroadPhaseInput.PotentialCollisionPairings = &SpatialHashingOutput.ShapePairs;
			CurrentBroadPhaseCollisionDetection->DetectCollision(&BroadPhaseInput, &BroadPhaseOutput, CurrentCollisionDetectionHandler);
			CurrentBroadPhaseCollisionDetection->DrawDebug(&BroadPhaseInput, &BroadPhaseOutput, nullptr, GetWorld(), TimeTick);
		}

		// 2. Solve Linear & Angular Physics
		
		// These steps calculate the velocities that will be applied to the object. By solving linear and angular
		// physics first, you get the raw velocities that are then used in integration.
		
		// if (CurrentLinearSolver)
		// {
		// 	CurrentLinearSolver->Solve(&LinearInput, &LinearOutput, Origin, TimeTick);
		// }
		//
		// if (CurrentAngularSolver)
		// {
		// 	CurrentAngularSolver->Solve(&AngularInput, &AngularOutput, Origin, TimeTick);
		// }
		//
		// // 3. @todo Solve Integration
		//
		// // This part of the solver takes the results from the linear and angular solvers and updates the position and
		// // orientation of objects over time. It essentially integrates the calculated forces and torques to determine
		// // how an object should move in the next time step.
		//
		// // 4. @todo Record state of all objects post-integration for replay purposes
		// // if (bIsRecording)
		// // {
		// // 	RecordPhysicsState();  // Implement this function to record the positions, velocities, etc.
		// // }
		//
		// // 4.5 @todo Override the physics engine's output with recorded data
		// // if (bIsReplaying)
		// // {
		// // 	ReplayPhysicsState();  // Implement this function to apply the recorded states
		// // }
		//
		// if (CurrentReplaySystem)
		// {
		// 	CurrentReplaySystem->RecordPhysicsState(&RecordedData, GetWorld()->GetTimeSeconds(), Shapes, &LinearInput, &AngularInput);
		// 	CurrentLinearSolver->Solve(&LinearInput, &LinearOutput, Origin, TimeTick);
		// }
		//
		// // 5. Spatial Hashing - Re-Generate shape pairs, because the shapes have moved and narrow-phase is expensive
		// if (CurrentHashingSystem)
		// {
		// 	// @todo use mesh or actor TM probably
		// 	CurrentHashingSystem->Solve(&SpatialHashingInput, &SpatialHashingOutput, Origin, TimeTick);
		// }
		
		// 6. @todo Solve Narrow-Phase Collision
		// @todo add checks for nothing in nearby bucket, skip both broad phase and narrow phase if required
		// if (CurrentNarrowPhaseCollisionDetection)
		// {
		// 	CurrentNarrowPhaseCollisionDetection->DetectCollision(TempDevShapes, NarrowPhaseOutput);
		// 	CurrentNarrowPhaseCollisionDetection->DrawDebug(NarrowPhaseOutput, Output.AnimInstanceProxy);
		// }

		// This step checks for actual collisions using detailed geometry after the object has been moved.
		// It’s a more precise and computationally expensive check compared to the broad phase.

		// 7. @todo Solve Contact

		// After detecting a collision, this step resolves it by adjusting the object's position and velocities. It
		// prevents interpenetration and handles the physical response of the objects involved in the collision.

		// 8. @todo Solve Constraints (Multiple!)

		// This handles constraints that limit or define the relationships between objects, such as joints
		// (e.g., hinges or sliders) that allow or restrict certain movements between connected objects.
		
		// Constraints are applied last because they often need to override other physical behaviors. For example,
		// if accessory_01 is constrained to follow spine_01, the constraint solver will ensure this attachment is
		// respected, regardless of the results of other physics calculations. You might have multiple constraints to
		// solve, depending on the complexity of your simulation.
		PhysicsUpdate.FinalizeTick();
	}

	// for (ATetherEditorShapeActor* ShapeActor : ShapeActors)
	// {
	// 	ShapeActor->TestCollisions(ShapeActors);
	// }
}

TStatId UTetherEditorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTetherEditorSubsystem, STATGROUP_Tickables);
}
