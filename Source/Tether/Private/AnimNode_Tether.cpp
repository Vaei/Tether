// Copyright (c) Jared Taylor. All Rights Reserved.

#include "AnimNode_Tether.h"

#include "TetherSettings.h"
#include "TetherStatics.h"
#include "Animation/AnimInstanceProxy.h"
#include "Physics/Collision/TetherCollisionDetectionBroadPhase.h"
#include "Physics/Collision/TetherCollisionDetectionNarrowPhase.h"
#include "Physics/Hashing/TetherHashingSpatial.h"
#include "Physics/Replay/TetherReplay.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverLinear.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverAngular.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"
#include "Shapes/TetherShape_Capsule.h"

DECLARE_CYCLE_STAT(TEXT("Tether_Update"), STAT_TetherUpdate, STATGROUP_Tether);
DECLARE_CYCLE_STAT(TEXT("Tether_Evaluate"), STAT_TetherEval, STATGROUP_Tether);

// #if ENABLE_DRAW_DEBUG
// TAutoConsoleVariable<bool> CVarTetherDebug(TEXT("p.Tether.Debug"), false, TEXT("Draw Tether Debugging information"));
// #endif

void FAnimNode_Tether::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	Super::Initialize_AnyThread(Context);

	PhysicsUpdate = { SimulationFrameRate };

	auto Sphere0 = FTetherShape_AxisAlignedBoundingBox();
	auto Sphere1 = FTetherShape_Capsule();
	Shapes.Add(Sphere0);
	Shapes.Add(Sphere1);

	// // Generate pointer array
	// Algo::Transform(Shapes, ShapePtrs, [](FTetherShape& Shape)
	// {
	// 	return &Shape;
	// });

	// @todo Does this need to ever be re-assigned?
	SpatialHashingInput.Shapes = &ShapePtrs;
}

void FAnimNode_Tether::UpdateInternal(const FAnimationUpdateContext& Context)
{
	FAnimNode_SkeletalControlBase::UpdateInternal(Context);
	
	SCOPE_CYCLE_COUNTER(STAT_TetherUpdate);

	const USkeletalMeshComponent* MeshComponent = Context.AnimInstanceProxy->GetSkelMeshComponent();
	const AActor* Owner = MeshComponent ? MeshComponent->GetOwner() : nullptr;
	if (!IsValid(Owner))
	{
		return;
	}

	// Detect gameplay tag changes and grab the newly referenced object
	if (LastHashingSystem != HashingSystem)
	{
		LastHashingSystem = HashingSystem;
		CurrentHashingSystem = UTetherSettings::GetHashingSystem(HashingSystem);
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
}

void FAnimNode_Tether::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output,
	TArray<FBoneTransform>& OutBoneTransforms)
{
	SCOPE_CYCLE_COUNTER(STAT_TetherEval);
	
	// Cache vars for use
	const USkeletalMeshComponent* MeshComponent = Output.AnimInstanceProxy->GetSkelMeshComponent();
	const FBoneContainer& RequiredBones = Output.AnimInstanceProxy->GetRequiredBones();
	const AActor* Owner = MeshComponent ? MeshComponent->GetOwner() : nullptr;
	const UWorld* World = Owner ? Owner->GetWorld() : nullptr;

	// Validate
	if (!MeshComponent || !IsValid(Owner) || !World)
	{
		return;
	}

	if (!RootBone.IsValidToEvaluate(RequiredBones))
	{
		return;
	}

	// This initializes the bone so that it can be modified; it is not merely grabbing a transform
	FTransform RootTM = Output.Pose.GetComponentSpaceTransform(RootBoneIndex);

	// Start the frame with the current DeltaTime
	PhysicsUpdate.StartFrame(Output.AnimInstanceProxy->GetDeltaSeconds());

	// Update at consistent framerate (default 60fps)
	while (PhysicsUpdate.ShouldTick())
	{
		// 0. Spatial Hashing - Generate shape pairs based on proximity and efficiency ratings for priority
		if (CurrentHashingSystem)
		{
			// @todo use mesh or actor TM probably
			CurrentHashingSystem->Solve(&SpatialHashingInput, &SpatialHashingOutput, RootTM, PhysicsUpdate.TimeTick);
			// CurrentHashingSystem->DrawDebug(&SpatialHashingInput, &SpatialHashingOutput, Output.AnimInstanceProxy, World);
		}
		
		// // 1. Solve Broad-Phase Collision
		// if (CurrentBroadPhaseCollisionDetection)
		// {
		// 	// Optional but common optimization step where you quickly check if objects are close enough to potentially
		// 	// collide. It reduces the number of detailed collision checks needed in the narrow phase.
		//
		// 	CurrentBroadPhaseCollisionDetection->DetectCollision(Shapes, BroadPhaseOutput);
		// 	CurrentBroadPhaseCollisionDetection->DrawDebug(Shapes, BroadPhaseOutput, Output.AnimInstanceProxy);
		// }

		// 2. Solve Linear & Angular Physics
		
		// These steps calculate the velocities that will be applied to the object. By solving linear and angular
		// physics first, you get the raw velocities that are then used in integration.
		
		if (CurrentLinearSolver)
		{
			CurrentLinearSolver->Solve(&LinearInput, &LinearOutput, RootTM, PhysicsUpdate.TimeTick);
		}
		
		if (CurrentAngularSolver)
		{
			CurrentAngularSolver->Solve(&AngularInput, &AngularOutput, RootTM, PhysicsUpdate.TimeTick);
		}

		// 3. @todo Solve Integration

		// This part of the solver takes the results from the linear and angular solvers and updates the position and
		// orientation of objects over time. It essentially integrates the calculated forces and torques to determine
		// how an object should move in the next time step.

		// 4. @todo Record state of all objects post-integration for replay purposes
		// if (bIsRecording)
		// {
		// 	RecordPhysicsState();  // Implement this function to record the positions, velocities, etc.
		// }

		// 4.5 @todo Override the physics engine's output with recorded data
		// if (bIsReplaying)
		// {
		// 	ReplayPhysicsState();  // Implement this function to apply the recorded states
		// }

		if (CurrentReplaySystem)
		{
			CurrentReplaySystem->RecordPhysicsState(&RecordedData, World->GetTimeSeconds(), Shapes, &LinearInput, &AngularInput);
			CurrentLinearSolver->Solve(&LinearInput, &LinearOutput, RootTM, PhysicsUpdate.TimeTick);
		}
		
		// 5. Spatial Hashing - Re-Generate shape pairs, because the shapes have moved and narrow-phase is expensive
		if (CurrentHashingSystem)
		{
			// @todo use mesh or actor TM probably
			CurrentHashingSystem->Solve(&SpatialHashingInput, &SpatialHashingOutput, RootTM, PhysicsUpdate.TimeTick);
		}
		
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
}

bool FAnimNode_Tether::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	if (!RootBone.IsValidToEvaluate(RequiredBones))
	{
		return false;
	}
	return true;
}

void FAnimNode_Tether::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	RootBone.Initialize(RequiredBones);
	RootBoneIndex = RootBone.GetCompactPoseIndex(RequiredBones);
}
