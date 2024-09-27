// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherIO.h"
#include "TetherPhysicsTypes.generated.h"

class UTetherContactSolver;
class UTetherCollisionDetectionNarrowPhase;
class UTetherCollisionDetectionBroadPhase;
class UTetherCollisionDetectionHandler;
class UTetherHashing;
class UTetherReplay;
class UTetherIntegrationSolver;
class UTetherPhysicsSolverAngular;
class UTetherPhysicsSolverLinear;
class UTetherActivityStateHandler;

/**
 * Convenience struct for handling detection and changing of common global solver types
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherCommonSharedSolverData
{
	GENERATED_BODY()

	FTetherCommonSharedSolverData()
	{}

public:
	UPROPERTY(Transient)
	const UTetherHashing* CurrentHashingSystem = nullptr;
		
	UPROPERTY(Transient)
	const UTetherCollisionDetectionHandler* CurrentCollisionDetectionHandler = nullptr;
	
	UPROPERTY(Transient)
	const UTetherCollisionDetectionBroadPhase* CurrentBroadPhaseCollisionDetection = nullptr;

	UPROPERTY(Transient)
	const UTetherCollisionDetectionNarrowPhase* CurrentNarrowPhaseCollisionDetection = nullptr;

protected:
	UPROPERTY(Transient)
	FGameplayTag LastHashingSystem = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastCollisionDetectionHandler = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastBroadPhaseCollisionDetection = FGameplayTag::EmptyTag;
	
	UPROPERTY(Transient)
	FGameplayTag LastNarrowPhaseCollisionDetection = FGameplayTag::EmptyTag;

public:
	void UpdateSolverData(const FGameplayTag& HashingSystem, const FGameplayTag& CollisionDetectionHandler,
	const FGameplayTag& BroadPhaseCollisionDetection, const FGameplayTag& NarrowPhaseCollisionDetection);
};

/**
 * These structs are split apart because some wrappers will want to provide their own gameplay tags
 * e.g. An anim graph node will want to have these tags on the node and exposed to bindings with PinHiddenByDefault
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherCommonSharedSolvers : public FTetherCommonSharedSolverData
{
	GENERATED_BODY()

	FTetherCommonSharedSolvers()
		: HashingSystem(FTetherGameplayTags::Tether_Hashing_Spatial)
		, CollisionDetectionHandler(FTetherGameplayTags::Tether_Detection_CollisionHandler)
		, BroadPhaseCollisionDetection(FTetherGameplayTags::Tether_Detection_BroadPhase)
		, NarrowPhaseCollisionDetection(FTetherGameplayTags::Tether_Detection_NarrowPhase)
	{}

	FTetherCommonSharedSolvers(const FGameplayTag& InHashingSystem, const FGameplayTag& InCollisionDetectionHandler,
	const FGameplayTag& InBroadPhaseCollisionDetection, const FGameplayTag& InNarrowPhaseCollisionDetection)
		: HashingSystem(InHashingSystem)
		, CollisionDetectionHandler(InCollisionDetectionHandler)
		, BroadPhaseCollisionDetection(InBroadPhaseCollisionDetection)
		, NarrowPhaseCollisionDetection(InNarrowPhaseCollisionDetection)
	{}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Hashing"))
	FGameplayTag HashingSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.CollisionHandler"))
	FGameplayTag CollisionDetectionHandler;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.BroadPhase"))
	FGameplayTag BroadPhaseCollisionDetection;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.NarrowPhase"))
	FGameplayTag NarrowPhaseCollisionDetection;

public:
	void UpdateSolvers();
};

USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherCommonSharedData
{
	GENERATED_BODY()

	FTetherCommonSharedData()
	{}
	
	FTetherCommonSharedData(const FSpatialHashingInput& InSpatialHashingInput, const FBroadPhaseInput& InBroadPhaseInput, const FNarrowPhaseInput& InNarrowPhaseInput)
		: SpatialHashingInput(InSpatialHashingInput)
		, BroadPhaseInput(InBroadPhaseInput)
		, NarrowPhaseInput(InNarrowPhaseInput)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FTetherCommonSharedSolvers Solvers;

	// Inputs
		
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	FSpatialHashingInput SpatialHashingInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FBroadPhaseInput BroadPhaseInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FNarrowPhaseInput NarrowPhaseInput;

	// Outputs
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FBroadPhaseOutput BroadPhaseOutput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FNarrowPhaseOutput NarrowPhaseOutput;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FSpatialHashingOutput SpatialHashingOutput;

	void InitializeSharedData()
	{
		BroadPhaseInput.PotentialCollisionPairings = &SpatialHashingOutput.ShapePairs;
		NarrowPhaseInput.CollisionPairings = &BroadPhaseOutput.CollisionPairings;
	}
};

/**
 * Convenience struct for handling detection and changing of common per-shape solver types
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherCommonShapeSolverData
{
	GENERATED_BODY()

	FTetherCommonShapeSolverData()
		: CurrentActivityStateHandler(nullptr)
		, CurrentLinearSolver(nullptr)
		, CurrentAngularSolver(nullptr)
		, CurrentIntegrationSolver(nullptr)
		, LastActivityStateHandler(FGameplayTag::EmptyTag)
		, LastLinearSolver(FGameplayTag::EmptyTag)
		, LastAngularSolver(FGameplayTag::EmptyTag)
		, LastIntegrationSolver(FGameplayTag::EmptyTag)
	{}

	UPROPERTY(Transient)
	const UTetherActivityStateHandler* CurrentActivityStateHandler;
	
	UPROPERTY(Transient)
	const UTetherPhysicsSolverLinear* CurrentLinearSolver;
	
	UPROPERTY(Transient)
	const UTetherPhysicsSolverAngular* CurrentAngularSolver;

	UPROPERTY(Transient)
	const UTetherIntegrationSolver* CurrentIntegrationSolver;
	
	UPROPERTY(Transient)
	const UTetherReplay* CurrentReplaySystem = nullptr;

	UPROPERTY(Transient)
	const UTetherContactSolver* CurrentContactSolver = nullptr;

protected:
	UPROPERTY(Transient)
	FGameplayTag LastActivityStateHandler = FGameplayTag::EmptyTag;
	
	UPROPERTY(Transient)
	FGameplayTag LastLinearSolver = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastAngularSolver = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastIntegrationSolver = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastReplaySystem = FGameplayTag::EmptyTag;
	
	UPROPERTY(Transient)
	FGameplayTag LastContactSolver = FGameplayTag::EmptyTag;

public:
	void UpdateSolverData(const FGameplayTag& ActivityStateHandler, const FGameplayTag& LinearSolver,
	const FGameplayTag& AngularSolver, const FGameplayTag& IntegrationSolver, const FGameplayTag& ReplaySystem,
	const FGameplayTag& ContactSolver);
};

/**
 * These structs are split apart because some wrappers will want to provide their own gameplay tags
 * e.g. An anim graph node will want to have these tags on the node and exposed to bindings with PinHiddenByDefault
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherCommonShapeSolvers : public FTetherCommonShapeSolverData
{
	GENERATED_BODY()

	FTetherCommonShapeSolvers()
		: ActivityStateHandler(FTetherGameplayTags::Tether_ActivityState)
		, LinearSolver(FTetherGameplayTags::Tether_Solver_Linear)
		, AngularSolver(FTetherGameplayTags::Tether_Solver_Angular)
		, IntegrationSolver(FTetherGameplayTags::Tether_Solver_Integration_Euler)
		, ContactSolver(FTetherGameplayTags::Tether_Solver_Contact_RigidBody_ImpulseVelocityLevel)
	{}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.ActivityState"))
	FGameplayTag ActivityStateHandler;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Linear"))
	FGameplayTag LinearSolver;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Angular"))
	FGameplayTag AngularSolver;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Integration"))
	FGameplayTag IntegrationSolver;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Replay"))
	FGameplayTag ReplaySystem = FTetherGameplayTags::Tether_Replay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Contact"))
	FGameplayTag ContactSolver;

public:
	void UpdateSolvers();
};

USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherCommonShapeData
{
	GENERATED_BODY()

	FTetherCommonShapeData()
	{}
	
	FTetherCommonShapeData(const FLinearInput& InLinearInput, const FAngularInput& InAngularInput, const FActivityStateInput& InActivityInput)
		: ActivityInput(InActivityInput)
		, LinearInput(InLinearInput)
		, AngularInput(InAngularInput)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FTetherCommonShapeSolvers Solvers;

	// Inputs
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FActivityStateInput ActivityInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FLinearInput LinearInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FAngularInput AngularInput;

	// Outputs

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Tether)
	FLinearOutput LinearOutput;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Tether)
	FAngularOutput AngularOutput;

	FIntegrationInput IntegrationInput;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Tether)
	FIntegrationOutput IntegrationOutput;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Tether)
	FRecordedPhysicsData RecordedData;

	void InitializeShapeData()
	{
		IntegrationInput.LinearInput = &LinearInput;
		IntegrationInput.LinearOutput = &LinearOutput;
		IntegrationInput.AngularInput = &AngularInput;
		IntegrationInput.AngularOutput = &AngularOutput;
	}
};