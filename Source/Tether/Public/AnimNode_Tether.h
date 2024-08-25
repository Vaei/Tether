﻿// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TetherGameplayTags.h"
#include "TetherIO.h"
#include "TetherPhysicsUpdate.h"
#include "TetherSpatialHashing.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "AnimNode_Tether.generated.h"

class UTetherCollisionDetectionNarrowPhase;
class UTetherCollisionDetectionBroadPhase;
class UTetherDeveloperSettings;
class UTetherPhysicsSolverAngular;
class UTetherPhysicsSolverLinear;
/**
 * Tether's core functionality
 */
USTRUCT(BlueprintInternalUseOnly)
struct TETHER_API FAnimNode_Tether : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Physics.Linear"))
	FGameplayTag LinearSolver = FTetherGameplayTags::Tether_Solver_Physics_Linear;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Physics.Angular"))
	FGameplayTag AngularSolver = FTetherGameplayTags::Tether_Solver_Physics_Angular;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Contact"))
	FGameplayTag ContactSolver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FTetherSpatialHashingInput SpatialHashingInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault))
	FLinearInput LinearInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault))
	FAngularInput AngularInput;

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
	UPROPERTY()
	TArray<FTetherShape> Shapes;  // @todo remove
	
	UPROPERTY()
	FLinearOutput LinearOutput;

	UPROPERTY()
	FAngularOutput AngularOutput;

	UPROPERTY()
	FTetherBroadPhaseCollisionOutput BroadPhaseOutput;

	UPROPERTY()
	TArray<FTetherNarrowPhaseCollisionOutput> NarrowPhaseOutput;

	UPROPERTY(Transient)
	FGameplayTag LastLinearSolver = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastAngularSolver = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	const UTetherCollisionDetectionBroadPhase* CurrentBroadPhaseCollisionDetection = nullptr;

	UPROPERTY(Transient)
	const UTetherCollisionDetectionNarrowPhase* CurrentNarrowPhaseCollisionDetection = nullptr;
	
	UPROPERTY(Transient)
	const UTetherPhysicsSolverLinear* CurrentLinearSolver = nullptr;
	
	UPROPERTY(Transient)
	const UTetherPhysicsSolverAngular* CurrentAngularSolver = nullptr;

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

	static const UTetherDeveloperSettings* GetSettings();
};
