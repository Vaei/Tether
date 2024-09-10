// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherEditorSubsystem.h"

#include "TetherDataAsset.h"
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

	const TSoftObjectPtr<UTetherDataAsset>& SoftData = UTetherSettings::Get()->EditorSubsystemDataAsset;
	if (SoftData.IsValid())
	{
		Data = SoftData.LoadSynchronous();
	}

	if (!ensureAlways(Data))
	{
		// Can never run
		return;
	}

	bHasWorldBegunPlay = true;

	SpatialHashingInput = Data->SpatialHashingInput;

	PhysicsUpdate = { Data->SimulationFrameRate };

	// Match render tick to physics tick, if the engine can keep up
	if (FTether::CVarTetherMatchFramerateToSimRate.GetValueOnGameThread())
	{
		if (GEngine)
		{
			// GEngine->SetMaxFPS(SimulationFrameRate);  // We don't use this, because we can't then clear it easily
	
			FString Command = FString::Printf(TEXT("t.maxfps %f"), Data->SimulationFrameRate);
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
	if (LastHashingSystem != Data->HashingSystem)
	{
		LastHashingSystem = Data->HashingSystem;
		CurrentHashingSystem = UTetherSettings::GetHashingSystem(Data->HashingSystem);
	}
	
	if (LastCollisionDetectionHandler != Data->CollisionDetectionHandler)
	{
		LastCollisionDetectionHandler = Data->CollisionDetectionHandler;
		CurrentCollisionDetectionHandler = UTetherSettings::GetCollisionDetectionHandler(Data->CollisionDetectionHandler);
	}

	if (!ensure(CurrentCollisionDetectionHandler))
	{
		return false;
	}
	
	if (LastBroadPhaseCollisionDetection != Data->BroadPhaseCollisionDetection)
	{
		LastBroadPhaseCollisionDetection = Data->BroadPhaseCollisionDetection;
		CurrentBroadPhaseCollisionDetection = UTetherSettings::GetBroadPhaseSystem(Data->BroadPhaseCollisionDetection);
	}
	
	if (LastActivityStateHandler != Data->ActivityStateHandler)
	{
		LastActivityStateHandler = Data->ActivityStateHandler;
		CurrentActivityStateHandler = UTetherSettings::GetActivityStateHandler<UTetherActivityStateHandler>(Data->ActivityStateHandler);
	}

	if (LastLinearSolver != Data->LinearSolver)
	{
		LastLinearSolver = Data->LinearSolver;
		CurrentLinearSolver = UTetherSettings::GetLinearPhysicsSolver<UTetherPhysicsSolverLinear>(Data->LinearSolver);
	}

	if (LastAngularSolver != Data->AngularSolver)
	{
		LastAngularSolver = Data->AngularSolver;
		CurrentAngularSolver = UTetherSettings::GetAngularPhysicsSolver<UTetherPhysicsSolverAngular>(Data->AngularSolver);
	}

	if (LastIntegrationSolver != Data->IntegrationSolver)
	{
		LastIntegrationSolver = Data->IntegrationSolver;
		CurrentIntegrationSolver = UTetherSettings::GetIntegrationSolver<UTetherIntegrationSolver>(Data->IntegrationSolver);
	}

	if (LastReplaySystem != Data->ReplaySystem)
	{
		LastReplaySystem = Data->ReplaySystem;
		CurrentReplaySystem = UTetherSettings::GetReplaySystem(Data->ReplaySystem);
	}

	if (LastNarrowPhaseCollisionDetection != Data->NarrowPhaseCollisionDetection)
	{
		LastNarrowPhaseCollisionDetection = Data->NarrowPhaseCollisionDetection;
		CurrentNarrowPhaseCollisionDetection = UTetherSettings::GetNarrowPhaseSystem(Data->NarrowPhaseCollisionDetection);
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
	TArray<FTetherShape*> Shapes;
	TArray<FVector> Origins;
	TMap<const FTetherShape*, ATetherEditorShapeActor*> ShapeActorMap;
	TMap<const FTetherShape*, const FTransform*> ShapeTransforms;
	for (ATetherEditorShapeActor* Actor : ShapeActors)
	{
		// Cache transform for computing origin
		Origins.Add(Actor->GetActorLocation());
		
		// Grab Shape
		FTetherShape* Shape = Actor->GetTetherShape();

		// Convert to world space
		Shape->ToWorldSpace(Actor->GetActorTransform());

		// Add to Shapes Array
		Shapes.Add(Shape);

		// Add to ShapeActorMap TMap
		ShapeActorMap.Add(Shape, Actor);

		// Add to ShapeTransforms TMap
		ShapeTransforms.Add(Shape, &Actor->GetActorTransform());

		// Cache LinearInputs
		FLinearInputSettings& LinearSettings = LinearInput.ShapeSettings.FindOrAdd(Shape);
		LinearSettings = Actor->LinearInputSettings;

		// Cache AngularInputs
		FAngularInputSettings& AngularSettings = AngularInput.ShapeSettings.FindOrAdd(Shape);
		AngularSettings = Actor->AngularInputSettings;
	}

	// Pass shapes to Inputs
	SpatialHashingInput.Shapes = &Shapes;
	BroadPhaseInput.Shapes = &Shapes;
	NarrowPhaseInput.Shapes = &Shapes;
	NarrowPhaseInput.CollisionPairings = &BroadPhaseOutput.CollisionPairings;
	ActivityStateHandlerInput.Shapes = &Shapes;
	ActivityStateHandlerInput.Settings = &Data->ActivitySettings;
	IntegrationInput.Shapes = &ShapeTransforms;
	IntegrationInput.LinearInput = &LinearInput;
	IntegrationInput.LinearOutput = &LinearOutput;
	IntegrationInput.AngularInput = &AngularInput;
	IntegrationInput.AngularOutput = &AngularOutput;
	IntegrationOutput.Shapes.Reset();  // Reset to re-init from input
	RecordedData.Shapes = &Shapes;
	
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

		// Spatial Hashing - Generate shape pairs based on proximity and efficiency ratings for priority
		if (CurrentHashingSystem)
		{
			CurrentHashingSystem->Solve(&SpatialHashingInput, &SpatialHashingOutput, Origin, TimeTick);
			CurrentHashingSystem->DrawDebug(&SpatialHashingInput, &SpatialHashingOutput, Origin, &DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
		}
		
		// Solve Broad-Phase Collision
		if (CurrentBroadPhaseCollisionDetection)
		{
			// Optional but common optimization step where you quickly check if objects are close enough to potentially
			// collide. It reduces the number of detailed collision checks needed in the narrow phase.
			BroadPhaseInput.PotentialCollisionPairings = &SpatialHashingOutput.ShapePairs;
			BroadPhaseInput.WorldTime = WorldTime;
			CurrentBroadPhaseCollisionDetection->DetectCollision(&BroadPhaseInput, &BroadPhaseOutput, CurrentCollisionDetectionHandler);
			CurrentBroadPhaseCollisionDetection->DrawDebug(&BroadPhaseInput, &BroadPhaseOutput, &DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
		}

		// Pre-Solve Activity State (Wake)
		if (CurrentActivityStateHandler)
		{
			ActivityStateHandlerInput.WorldTime = WorldTime;
			CurrentActivityStateHandler->PreSolveWake(&ActivityStateHandlerInput, &LinearInput, &AngularInput, DeltaTime);
		}

		// Solve Linear & Angular Physics
		
		// These steps calculate the velocities that will be applied to the object. By solving linear and angular
		// physics first, you get the raw velocities that are then used in integration.
		
		if (CurrentLinearSolver)
		{
			LinearInput.WorldTime = WorldTime;
			CurrentLinearSolver->Solve(&LinearInput, &LinearOutput, Origin, TimeTick);
			CurrentLinearSolver->DrawDebug(&LinearInput, &LinearOutput, ShapeTransforms, &DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
		}
		
		if (CurrentAngularSolver)
		{
			CurrentAngularSolver->Solve(&AngularInput, &AngularOutput, Origin, TimeTick);
			CurrentAngularSolver->DrawDebug(&AngularInput, &AngularOutput, ShapeTransforms, &DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld(),
				FColor::Emerald, FColor::Cyan, FColor::Orange);
		}

		// Post-Solve Activity State (Sleep)
		if (CurrentActivityStateHandler)
		{
			CurrentActivityStateHandler->PostSolveSleep(&ActivityStateHandlerInput, &LinearInput, &AngularInput, &LinearOutput, &AngularOutput, DeltaTime);
			CurrentActivityStateHandler->DrawDebug(&ActivityStateHandlerInput, ShapeTransforms, &DebugTextService.PendingDebugText, TimeTick, nullptr, GetWorld());
		}
		
		// Solve Integration
		
		// This part of the solver takes the results from the linear and angular solvers and updates the position and
		// orientation of objects over time. It essentially integrates the calculated forces and torques to determine
		// how an object should move in the next time step.

		if (CurrentIntegrationSolver)
		{
			CurrentIntegrationSolver->Solve(&IntegrationInput, &IntegrationOutput, TimeTick);
		}
		
		// @todo Record state of all objects post-integration for replay purposes

		// @todo Override the physics engine's output with recorded data

		// if (CurrentReplaySystem)
		// {
		// 	CurrentReplaySystem->RecordPhysicsState(&RecordedData, WorldTime, &LinearInput, &AngularInput);
		// }

		// @todo Solve Narrow-Phase Collision
		// @todo add checks for nothing in nearby bucket, skip both broad phase and narrow phase if required
		if (CurrentNarrowPhaseCollisionDetection)
		{
			CurrentNarrowPhaseCollisionDetection->DetectCollision(&NarrowPhaseInput, &NarrowPhaseOutput);
			// CurrentNarrowPhaseCollisionDetection->DrawDebug(NarrowPhaseOutput, Output.AnimInstanceProxy);
		}

		// This step checks for actual collisions using detailed geometry after the object has been moved.
		// It’s a more precise and computationally expensive check compared to the broad phase.

		// @todo Solve Contact

		// After detecting a collision, this step resolves it by adjusting the object's position and velocities. It
		// prevents interpenetration and handles the physical response of the objects involved in the collision.

		// @todo Solve Constraints (Multiple!)

		// This handles constraints that limit or define the relationships between objects, such as joints
		// (e.g., hinges or sliders) that allow or restrict certain movements between connected objects.
		
		// Constraints are applied last because they often need to override other physical behaviors. For example,
		// if accessory_01 is constrained to follow spine_01, the constraint solver will ensure this attachment is
		// respected, regardless of the results of other physics calculations. You might have multiple constraints to
		// solve, depending on the complexity of your simulation.

		WorldTime += TimeTick;
		PhysicsUpdate.FinalizeTick();
	}
	
	// Update actual transforms based on integration result
	for (const auto& ActorItr : ShapeActorMap)
	{
		const FTetherShape* const& Shape = ActorItr.Key;
		ATetherEditorShapeActor* Actor = ActorItr.Value;
		if (ensure(IsValid(Actor)))
		{
			const FTransform* Transform = IntegrationOutput.Shapes.Find(Shape);
			if (ensure(Transform))
			{
				Actor->SetActorTransform(*Transform);
			}
		}
	}

	// Print any pending messages
	MessageLog.ProcessMessages();
}

TStatId UTetherEditorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTetherEditorSubsystem, STATGROUP_Tickables);
}