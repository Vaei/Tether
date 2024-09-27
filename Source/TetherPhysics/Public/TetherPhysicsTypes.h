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
 * Convenience struct for handling detection and switching of common global solver types.
 * 
 * FTetherCommonSharedSolverData is responsible for managing the current state of key solver components
 * such as hashing systems, collision detection, and broad/narrow phase systems. 
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherCommonSharedSolverData
{
	GENERATED_BODY()

	FTetherCommonSharedSolverData()
	{}

public:
	/** The currently active spatial hashing system used in the simulation. */
	UPROPERTY(Transient)
	const UTetherHashing* CurrentHashingSystem = nullptr;
	
	/** The currently active collision detection handler. */
	UPROPERTY(Transient)
	const UTetherCollisionDetectionHandler* CurrentCollisionDetectionHandler = nullptr;

	/** The currently active broad-phase collision detection system. */
	UPROPERTY(Transient)
	const UTetherCollisionDetectionBroadPhase* CurrentBroadPhaseCollisionDetection = nullptr;

	/** The currently active narrow-phase collision detection system. */
	UPROPERTY(Transient)
	const UTetherCollisionDetectionNarrowPhase* CurrentNarrowPhaseCollisionDetection = nullptr;

protected:
	/** Stores the last used gameplay tag for hashing systems. */
	UPROPERTY(Transient)
	FGameplayTag LastHashingSystem = FGameplayTag::EmptyTag;

	/** Stores the last used gameplay tag for collision detection handlers. */
	UPROPERTY(Transient)
	FGameplayTag LastCollisionDetectionHandler = FGameplayTag::EmptyTag;

	/** Stores the last used gameplay tag for broad-phase collision detection. */
	UPROPERTY(Transient)
	FGameplayTag LastBroadPhaseCollisionDetection = FGameplayTag::EmptyTag;
	
	/** Stores the last used gameplay tag for narrow-phase collision detection. */
	UPROPERTY(Transient)
	FGameplayTag LastNarrowPhaseCollisionDetection = FGameplayTag::EmptyTag;

public:
	/** Updates the solver data based on the current gameplay tags provided for each component. */
	void UpdateSolverData(const FGameplayTag& HashingSystem, const FGameplayTag& CollisionDetectionHandler,
	const FGameplayTag& BroadPhaseCollisionDetection, const FGameplayTag& NarrowPhaseCollisionDetection);
};

/**
 * Wrapper struct for handling shared solver components in the Tether physics system.
 *
 * FTetherCommonSharedSolvers builds on FTetherCommonSharedSolverData, allowing for flexible updates and management
 * of key solver types such as hashing, collision detection, and contact resolution. These solvers are tied to 
 * gameplay tags, making it easier to switch or modify them based on gameplay conditions.
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
	/** The gameplay tag associated with the current spatial hashing system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Hashing"))
	FGameplayTag HashingSystem;

	/** The gameplay tag associated with the current collision detection handler. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.CollisionHandler"))
	FGameplayTag CollisionDetectionHandler;
	
	/** The gameplay tag associated with the current broad-phase collision detection system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.BroadPhase"))
	FGameplayTag BroadPhaseCollisionDetection;
		
	/** The gameplay tag associated with the current narrow-phase collision detection system. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.NarrowPhase"))
	FGameplayTag NarrowPhaseCollisionDetection;

public:
	/** Updates all solver components based on the current gameplay tags. */
	void UpdateSolvers();
};

/**
 * Shared data structure used in the Tether physics system for managing spatial hashing, 
 * collision detection, and solver interactions.
 *
 * FTetherCommonSharedData aggregates the inputs and outputs for various stages of the physics
 * simulation, including spatial hashing, broad-phase, and narrow-phase collision detection.
 * 
 * This structure is responsible for managing the flow of data between these stages.
 */
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

	/** Shared solvers used by the physics system (hashing, collision detection, etc.). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FTetherCommonSharedSolvers Solvers;

	// Inputs
		
	/** Input data for spatial hashing. */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	FSpatialHashingInput SpatialHashingInput;

	/** Input data for broad-phase collision detection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FBroadPhaseInput BroadPhaseInput;

	/** Input data for narrow-phase collision detection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FNarrowPhaseInput NarrowPhaseInput;

	// Outputs
	
	/** Output data from broad-phase collision detection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FBroadPhaseOutput BroadPhaseOutput;

	/** Output data from narrow-phase collision detection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FNarrowPhaseOutput NarrowPhaseOutput;
	
	/** Output data from spatial hashing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FSpatialHashingOutput SpatialHashingOutput;

	/** Initializes shared data and links inputs and outputs between stages. */
	void InitializeSharedData()
	{
		BroadPhaseInput.PotentialCollisionPairings = &SpatialHashingOutput.ShapePairs;
		NarrowPhaseInput.CollisionPairings = &BroadPhaseOutput.CollisionPairings;
	}
};

/**
 * Convenience struct for handling detection and switching of per-shape solver types.
 *
 * FTetherCommonShapeSolverData manages the current state of solvers that apply specifically to each shape,
 * such as linear and angular solvers, activity state handlers, and integration solvers.
 * 
 * This allows for fine-grained control over the behavior of individual shapes in the physics system.
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

	/** The currently active activity state handler for this shape. */
	UPROPERTY(Transient)
	const UTetherActivityStateHandler* CurrentActivityStateHandler;
	
	/** The currently active linear physics solver for this shape. */
	UPROPERTY(Transient)
	const UTetherPhysicsSolverLinear* CurrentLinearSolver;
	
	/** The currently active angular physics solver for this shape. */
	UPROPERTY(Transient)
	const UTetherPhysicsSolverAngular* CurrentAngularSolver;

	/** The currently active integration solver for this shape. */
	UPROPERTY(Transient)
	const UTetherIntegrationSolver* CurrentIntegrationSolver;
	
	/** The currently active replay system for this shape. */
	UPROPERTY(Transient)
	const UTetherReplay* CurrentReplaySystem = nullptr;

	/** The currently active contact solver for this shape. */
	UPROPERTY(Transient)
	const UTetherContactSolver* CurrentContactSolver = nullptr;

protected:
	/** Stores the last used gameplay tag for the activity state handler. */
	UPROPERTY(Transient)
	FGameplayTag LastActivityStateHandler = FGameplayTag::EmptyTag;
	
	/** Stores the last used gameplay tag for the linear solver. */
	UPROPERTY(Transient)
	FGameplayTag LastLinearSolver = FGameplayTag::EmptyTag;

	/** Stores the last used gameplay tag for the angular solver. */
	UPROPERTY(Transient)
	FGameplayTag LastAngularSolver = FGameplayTag::EmptyTag;

	/** Stores the last used gameplay tag for the integration solver. */
	UPROPERTY(Transient)
	FGameplayTag LastIntegrationSolver = FGameplayTag::EmptyTag;

	/** Stores the last used gameplay tag for the replay system. */
	UPROPERTY(Transient)
	FGameplayTag LastReplaySystem = FGameplayTag::EmptyTag;
	
	/** Stores the last used gameplay tag for the contact solver. */
	UPROPERTY(Transient)
	FGameplayTag LastContactSolver = FGameplayTag::EmptyTag;

public:
	/** Updates the solver data for a shape based on the provided gameplay tags. */
	void UpdateSolverData(const FGameplayTag& ActivityStateHandler, const FGameplayTag& LinearSolver,
	const FGameplayTag& AngularSolver, const FGameplayTag& IntegrationSolver, const FGameplayTag& ReplaySystem,
	const FGameplayTag& ContactSolver);
};

/**
 * Wrapper struct for managing per-shape solvers in the Tether physics system.
 *
 * FTetherCommonShapeSolvers builds on FTetherCommonShapeSolverData, allowing for the flexible and efficient
 * management of solvers specific to individual shapes.
 *
 * These solvers are linked to gameplay tags and allow shapes to be assigned different solver behaviors dynamically.
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
	/** The gameplay tag for the current activity state handler of the shape. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.ActivityState"))
	FGameplayTag ActivityStateHandler;
	
	/** The gameplay tag for the current linear physics solver of the shape. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Linear"))
	FGameplayTag LinearSolver;
	
	/** The gameplay tag for the current angular physics solver of the shape. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Angular"))
	FGameplayTag AngularSolver;
	
	/** The gameplay tag for the current integration solver of the shape. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Integration"))
	FGameplayTag IntegrationSolver;
	
	/** The gameplay tag for the current replay system of the shape. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Replay"))
	FGameplayTag ReplaySystem = FTetherGameplayTags::Tether_Replay;

	/** The gameplay tag for the current contact solver of the shape. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Contact"))
	FGameplayTag ContactSolver;

public:
	/** Updates all solvers for this shape based on the provided gameplay tags. */
	void UpdateSolvers();
};

/**
 * Structure for managing per-shape input/output data and solvers in the Tether physics system.
 *
 * FTetherCommonShapeData holds the state for linear and angular motion, activity state handling, and 
 * integration results.
 *
 * It also manages the solvers assigned to the shape and provides methods for initializing the shape's data for simulation.
 */
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


	/** Solvers used by this shape (linear, angular, etc.). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FTetherCommonShapeSolvers Solvers;

	// Inputs
	
	/** Input data for activity state handling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FActivityStateInput ActivityInput;

	/** Input data for linear motion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FLinearInput LinearInput;

	/** Input data for angular motion. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FAngularInput AngularInput;

	// Outputs

	/** Output data for linear motion. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Tether)
	FLinearOutput LinearOutput;

	/** Output data for angular motion. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Tether)
	FAngularOutput AngularOutput;

	/** Input data for integration (linear and angular motion combined). */
	FIntegrationInput IntegrationInput;

	/** Output data for integration results (new transforms). */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Tether)
	FIntegrationOutput IntegrationOutput;
	
	/** Recorded data for replaying the physics simulation of this shape. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Tether)
	FRecordedPhysicsData RecordedData;

	/** Initializes the shape's data, linking inputs and outputs for the simulation. */
	void InitializeShapeData()
	{
		IntegrationInput.LinearInput = &LinearInput;
		IntegrationInput.LinearOutput = &LinearOutput;
		IntegrationInput.AngularInput = &AngularInput;
		IntegrationInput.AngularOutput = &AngularOutput;
	}
};