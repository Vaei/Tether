// Copyright (c) 2023 Drowning Dragons. All Rights Reserved.

#include "AnimNode_Tether.h"

#include "TetherDeveloperSettings.h"
#include "TetherSpatialHashing.h"
#include "TetherStatics.h"
#include "Animation/AnimInstanceProxy.h"
#include "Physics/Collision/TetherCollisionDetectionBroadPhase.h"
#include "Physics/Collision/TetherCollisionDetectionNarrowPhase.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverLinear.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverAngular.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"
#include "Shapes/TetherShape_Capsule.h"

DECLARE_CYCLE_STAT(TEXT("Tether_Update"), STAT_TetherUpdate, STATGROUP_Tether);
DECLARE_CYCLE_STAT(TEXT("Tether_Evaluate"), STAT_TetherEval, STATGROUP_Tether);

// #if UE_ENABLE_DEBUG_DRAWING
// TAutoConsoleVariable<bool> CVarMorpherDebugOffsetIK(TEXT("a.Tether.DebugOffsetIK"), false, TEXT("Draw Morpher IK Debugging to Screen when HandlingMode == MaintainOffset"));
// TAutoConsoleVariable<bool> CVarMorpherDisableUserErrorChecking(TEXT("a.AnimNode.Morpher.DisableUserErrorChecking"), false, TEXT("Prevent user-error checking (overrides UserErrorNodeSeverity)"));
// #endif

void FAnimNode_Tether::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	Super::Initialize_AnyThread(Context);

	auto Sphere0 = FTetherShape_AxisAlignedBoundingBox();
	auto Sphere1 = FTetherShape_Capsule();
	Shapes.Add(Sphere0);
	Shapes.Add(Sphere1);
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

	if (LastLinearSolver != LinearSolver)
	{
		LastLinearSolver = LinearSolver;
		if (const TSubclassOf<UTetherPhysicsSolver>* NewSolverClass = GetSettings()->Solvers.Find(LinearSolver))
		{
			if (UTetherPhysicsSolver* NewSolver = NewSolverClass->GetDefaultObject())
			{
				CurrentLinearSolver = Cast<UTetherPhysicsSolverLinear>(NewSolver);
			}
		}
	}

	if (LastAngularSolver != AngularSolver)
	{
		LastAngularSolver = AngularSolver;
		if (const TSubclassOf<UTetherPhysicsSolver>* NewSolverClass = GetSettings()->Solvers.Find(AngularSolver))
		{
			if (UTetherPhysicsSolver* NewSolver = NewSolverClass->GetDefaultObject())
			{
				CurrentAngularSolver = Cast<UTetherPhysicsSolverAngular>(NewSolver);
			}
		}
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

	// Validate
	if (!MeshComponent || !IsValid(Owner))
	{
		return;
	}

	if (!RootBone.IsValidToEvaluate(RequiredBones))
	{
		return;
	}

	// This initializes the bone so that it can be modified; it is not merely grabbing a transform
	FTransform RootTM = Output.Pose.GetComponentSpaceTransform(RootBoneIndex);

	// Calculate the delta time for each physics sub-tick
	float TimeTick = 1.0f / static_cast<float>(SimulationFrameRate);

	// Accumulate the time since the last frame, ensuring any left-over time from last frame is included
	const float DeltaTime = Output.AnimInstanceProxy->GetDeltaSeconds();
	RemainingTime += DeltaTime;

	// Run physics sub-ticks to catch up with the accumulated time
	while (RemainingTime >= TimeTick)
	{
		// 0. Spatial Hashing - Generate shape pairs based on proximity and efficiency ratings for priority
		FTetherSpatialHashing SpatialHashing { SpatialHashingInput, Shapes, RootTM.GetLocation() };  // @todo this should be mesh center, I guess
		SpatialHashing.DrawDebugSpatialGrid(Output.AnimInstanceProxy, nullptr);

		// @todo 1. Solve Broad-Phase Collision

		// Optional but common optimization step where you quickly check if objects are close enough to potentially
		// collide. It reduces the number of detailed collision checks needed in the narrow phase.
		
		if (CurrentBroadPhaseCollisionDetection)
		{
			CurrentBroadPhaseCollisionDetection->DetectCollision(Shapes, BroadPhaseOutput);
			CurrentBroadPhaseCollisionDetection->DrawDebug(Shapes, BroadPhaseOutput, Output.AnimInstanceProxy);
		}

		// 2. Solve Linear & Angular Physics
		
		// These steps calculate the velocities that will be applied to the object. By solving linear and angular
		// physics first, you get the raw velocities that are then used in integration.
		
		if (CurrentLinearSolver)
		{
			CurrentLinearSolver->Solve(LinearInput, LinearOutput, RootTM, TimeTick);
		}
		
		if (CurrentAngularSolver)
		{
			static constexpr float DebugRadius = 25.f;
			CurrentAngularSolver->Solve(AngularInput, AngularOutput, RootTM, TimeTick, DebugRadius);
		}

		// @todo 4. Solve Integration

		// This part of the solver takes the results from the linear and angular solvers and updates the position and
		// orientation of objects over time. It essentially integrates the calculated forces and torques to determine
		// how an object should move in the next time step.

		// @todo 5. Solve Narrow-Phase Collision
		SpatialHashing.GenerateShapePairs(Shapes);  // Shapes have likely moved
		// @todo add checks for nothing in nearby bucket, skip both broad phase and narrow phase if required
		// if (CurrentNarrowPhaseCollisionDetection)
		// {
		// 	CurrentNarrowPhaseCollisionDetection->DetectCollision(TempDevShapes, NarrowPhaseOutput);
		// 	CurrentNarrowPhaseCollisionDetection->DrawDebug(NarrowPhaseOutput, Output.AnimInstanceProxy);
		// }

		// This step checks for actual collisions using detailed geometry after the object has been moved.
		// It’s a more precise and computationally expensive check compared to the broad phase.

		// @todo 6. Solve Contact

		// After detecting a collision, this step resolves it by adjusting the object's position and velocities. It
		// prevents interpenetration and handles the physical response of the objects involved in the collision.

		// @todo 7. Solve Constraints (Multiple!)

		// This handles constraints that limit or define the relationships between objects, such as joints
		// (e.g., hinges or sliders) that allow or restrict certain movements between connected objects.
		
		// Constraints are applied last because they often need to override other physical behaviors. For example,
		// if accessory_01 is constrained to follow spine_01, the constraint solver will ensure this attachment is
		// respected, regardless of the results of other physics calculations. You might have multiple constraints to
		// solve, depending on the complexity of your simulation.
		
		// Reduce the accumulator by the sub-tick delta time
		RemainingTime -= TimeTick;
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

const UTetherDeveloperSettings* FAnimNode_Tether::GetSettings()
{
	return GetDefault<UTetherDeveloperSettings>();
}
