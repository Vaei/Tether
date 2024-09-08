﻿// Copyright (c) Jared Taylor. All Rights Reserved.

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Hashing"))
	FGameplayTag HashingSystem = FTetherGameplayTags::Tether_Hashing_Spatial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Detection.CollisionHandler"))
	FGameplayTag CollisionDetectionHandler = FTetherGameplayTags::Tether_Detection_CollisionHandler;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Detection.BroadPhase"))
	FGameplayTag BroadPhaseCollisionDetection = FTetherGameplayTags::Tether_Detection_BroadPhase;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Detection.NarrowPhase"))
	FGameplayTag NarrowPhaseCollisionDetection = FTetherGameplayTags::Tether_Detection_NarrowPhase;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Physics.Linear"))
	FGameplayTag LinearSolver = FTetherGameplayTags::Tether_Solver_Physics_Linear;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Physics.Angular"))
	FGameplayTag AngularSolver = FTetherGameplayTags::Tether_Solver_Physics_Angular;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Integration"))
	FGameplayTag IntegrationSolver = FTetherGameplayTags::Tether_Solver_Integration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Replay"))
	FGameplayTag ReplaySystem = FTetherGameplayTags::Tether_Replay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Contact"))
	FGameplayTag ContactSolver = FTetherGameplayTags::Tether_Solver_Contact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FSpatialHashingInput SpatialHashingInput;

	/** 
	 * Target frame rate for the physics simulation. This value determines the frequency at which physics calculations
	 * are performed per second. A higher frame rate results in more accurate simulations but increases computational
	 * cost.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, ClampMin="1", UIMin="1", UIMax="120"))
	float SimulationFrameRate = 60.f;
};