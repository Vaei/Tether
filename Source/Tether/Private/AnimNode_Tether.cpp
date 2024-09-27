// Copyright (c) Jared Taylor. All Rights Reserved.

#include "AnimNode_Tether.h"

#include "TetherSettings.h"
#include "TetherStatics.h"
#include "Animation/AnimInstanceProxy.h"

DECLARE_CYCLE_STAT(TEXT("Tether_Update"), STAT_TetherUpdate, STATGROUP_Tether);
DECLARE_CYCLE_STAT(TEXT("Tether_Evaluate"), STAT_TetherEval, STATGROUP_Tether);

// #if ENABLE_DRAW_DEBUG
// TAutoConsoleVariable<bool> CVarTetherDebug(TEXT("p.Tether.Debug"), false, TEXT("Draw Tether Debugging information"));
// #endif

void FAnimNode_Tether::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	Super::Initialize_AnyThread(Context);

	PhysicsUpdate = { SimulationFrameRate };
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
