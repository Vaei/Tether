// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TetherGameplayTags.h"
#include "TetherIO.h"
#include "Engine/DataAsset.h"
#include "TetherDataAsset.generated.h"

/**
 * A data asset that holds the configuration and gameplay tags used in the Tether physics system.
 *
 * UTetherDataAsset serves as a container for common settings and gameplay tags required by the Tether system,
 * including hashing systems, collision detection systems, solvers, spatial hashing input, and activity settings.
 * It allows for centralized management of these parameters, which can be referenced throughout the physics engine.
 *
 * Tether does support _not_ using this. It is possible to set up the system without using this data asset.
 */
UCLASS()
class TETHERPHYSICS_API UTetherDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Hashing system, responsible for shape pairing based on proximity. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Hashing"))
	FGameplayTag HashingSystem = FTetherGameplayTags::Tether_Hashing_Spatial;

	/** Collision detection handler, responsible for executing overlap tests. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.CollisionHandler"))
	FGameplayTag CollisionDetectionHandler = FTetherGameplayTags::Tether_Detection_CollisionHandler;
	
	/** Broad-phase collision detection system, responsible for simplified overlap detection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.BroadPhase"))
	FGameplayTag BroadPhaseCollisionDetection = FTetherGameplayTags::Tether_Detection_BroadPhase;
		
	/** Narrow-phase collision detection system, responsible for complex overlap detection. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.NarrowPhase"))
	FGameplayTag NarrowPhaseCollisionDetection = FTetherGameplayTags::Tether_Detection_NarrowPhase;

	/** Contact solver, responsible for resolving object interactions during collisions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Contact"))
	FGameplayTag ContactSolver = FTetherGameplayTags::Tether_Solver_Contact;

	/** Input data for spatial hashing, defining the grid system for efficiently detecting potential collisions. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FSpatialHashingInput SpatialHashingInput;

	/** Configuration settings for controlling sleep/wake behavior based on activity thresholds (e.g., velocity, force). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FActivitySettings ActivitySettings;
	
	/** 
	 * Target frame rate for the physics simulation. This value determines how frequently physics calculations
	 * are performed per second. A higher frame rate results in more precise simulations but increases computational cost.
	 * It should be set according to the performance capabilities of the hardware running the simulation.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, ClampMin="1", UIMin="1", UIMax="120"))
	float SimulationFrameRate = 60.f;
};
