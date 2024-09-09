// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TetherGameplayTags.h"
#include "TetherIO.h"
#include "Engine/DataAsset.h"
#include "TetherDataAsset.generated.h"

/**
 * A data asset containing common properties used with Tether
 */
UCLASS()
class TETHERPHYSICS_API UTetherDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Hashing"))
	FGameplayTag HashingSystem = FTetherGameplayTags::Tether_Hashing_Spatial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.CollisionHandler"))
	FGameplayTag CollisionDetectionHandler = FTetherGameplayTags::Tether_Detection_CollisionHandler;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.BroadPhase"))
	FGameplayTag BroadPhaseCollisionDetection = FTetherGameplayTags::Tether_Detection_BroadPhase;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Detection.NarrowPhase"))
	FGameplayTag NarrowPhaseCollisionDetection = FTetherGameplayTags::Tether_Detection_NarrowPhase;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.ActivityState"))
	FGameplayTag ActivityStateHandler = FTetherGameplayTags::Tether_ActivityState;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Linear"))
	FGameplayTag LinearSolver = FTetherGameplayTags::Tether_Solver_Linear;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Angular"))
	FGameplayTag AngularSolver = FTetherGameplayTags::Tether_Solver_Angular;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Integration"))
	FGameplayTag IntegrationSolver = FTetherGameplayTags::Tether_Solver_Integration_Euler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Replay"))
	FGameplayTag ReplaySystem = FTetherGameplayTags::Tether_Replay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Solver.Contact"))
	FGameplayTag ContactSolver = FTetherGameplayTags::Tether_Solver_Contact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FSpatialHashingInput SpatialHashingInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FTetherActivitySettings ActivitySettings;
	
	/** 
	 * Target frame rate for the physics simulation. This value determines the frequency at which physics calculations
	 * are performed per second. A higher frame rate results in more accurate simulations but increases computational
	 * cost.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, ClampMin="1", UIMin="1", UIMax="120"))
	float SimulationFrameRate = 60.f;
};
