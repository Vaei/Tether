// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TetherGameplayTags.h"
#include "TetherIO.h"
#include "TetherPhysicsUpdate.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "AnimNode_Tether.generated.h"

class UTetherReplay;
class UTetherPhysicsSolverAngular;
class UTetherPhysicsSolverLinear;
class UTetherCollisionDetectionNarrowPhase;
class UTetherCollisionDetectionBroadPhase;
class UTetherHashing;

/**
 * Tether's core functionality
 */
USTRUCT(BlueprintInternalUseOnly)
struct TETHER_API FAnimNode_Tether : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Hashing"))
	FGameplayTag HashingSystem = FTetherGameplayTags::Tether_Hashing_Spatial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Linear"))
	FGameplayTag LinearSolver = FTetherGameplayTags::Tether_Solver_Linear;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Angular"))
	FGameplayTag AngularSolver = FTetherGameplayTags::Tether_Solver_Angular;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Replay"))
	FGameplayTag ReplaySystem = FTetherGameplayTags::Tether_Replay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Contact"))
	FGameplayTag ContactSolver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FSpatialHashingInput SpatialHashingInput;

	/** 
	 * Target frame rate for the physics simulation. This value determines the frequency at which physics calculations
	 * are performed per second. A higher frame rate results in more accurate simulations but increases computational
	 * cost.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, ClampMin="1", UIMin="1", UIMax="120"))
	float SimulationFrameRate = 60.f;

	UPROPERTY(EditAnywhere, Category=Tether)
	FBoneReference RootBone;
	
protected:
	/** Used to prevent Evaluate() running logic before the first update */
	UPROPERTY()
	bool bFirstUpdate = true;

	FTetherPhysicsUpdate PhysicsUpdate = { SimulationFrameRate };

protected:
	FCompactPoseBoneIndex RootBoneIndex = FCompactPoseBoneIndex(INDEX_NONE);
	
protected:
	// FAnimNode_SkeletalControlBase interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface
};
