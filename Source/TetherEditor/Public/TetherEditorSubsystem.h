// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TetherGameplayTags.h"
#include "TetherIO.h"
#include "Subsystems/WorldSubsystem.h"
#include "Tether/Public/TetherPhysicsUpdate.h"
#include "TetherEditorSubsystem.generated.h"

class UTetherCollisionDetectionHandler;
class UTetherReplay;
class UTetherPhysicsSolverAngular;
class UTetherPhysicsSolverLinear;
class UTetherCollisionDetectionNarrowPhase;
class UTetherCollisionDetectionBroadPhase;
class UTetherHashing;
class ATetherEditorShapeActor;

/**
 * Wrapper run Tether in the editor for testing purposes
 */
UCLASS()
class TETHEREDITOR_API UTetherEditorSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	static UTetherEditorSubsystem* Get(const UWorld* World)
	{
		return World ? World->GetSubsystem<UTetherEditorSubsystem>() : nullptr;
	}
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Replay"))
	FGameplayTag ReplaySystem = FTetherGameplayTags::Tether_Replay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(PinHiddenByDefault, Categories="Tether.Solver.Contact"))
	FGameplayTag ContactSolver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FSpatialHashingInput SpatialHashingInput;

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

protected:
	FTetherPhysicsUpdate PhysicsUpdate = { SimulationFrameRate };

	UPROPERTY()
	FSpatialHashingOutput SpatialHashingOutput;
	
	UPROPERTY()
	FLinearOutput LinearOutput;

	UPROPERTY()
	FAngularOutput AngularOutput;

	UPROPERTY()
	FRecordedPhysicsData RecordedData;
	
	UPROPERTY()
	FTetherBroadPhaseCollisionInput BroadPhaseInput;

	UPROPERTY()
	FTetherBroadPhaseCollisionOutput BroadPhaseOutput;

	UPROPERTY()
	TArray<FTetherNarrowPhaseCollisionOutput> NarrowPhaseOutput;

	UPROPERTY(Transient)
	FGameplayTag LastHashingSystem = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastCollisionDetectionHandler = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastBroadPhaseCollisionDetection = FGameplayTag::EmptyTag;
	
	UPROPERTY(Transient)
	FGameplayTag LastNarrowPhaseCollisionDetection = FGameplayTag::EmptyTag;
	
	UPROPERTY(Transient)
	FGameplayTag LastLinearSolver = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastAngularSolver = FGameplayTag::EmptyTag;
	
	UPROPERTY(Transient)
	FGameplayTag LastReplaySystem = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	const UTetherHashing* CurrentHashingSystem = nullptr;
		
	UPROPERTY(Transient)
	const UTetherCollisionDetectionHandler* CurrentCollisionDetectionHandler = nullptr;
	
	UPROPERTY(Transient)
	const UTetherCollisionDetectionBroadPhase* CurrentBroadPhaseCollisionDetection = nullptr;

	UPROPERTY(Transient)
	const UTetherCollisionDetectionNarrowPhase* CurrentNarrowPhaseCollisionDetection = nullptr;
	
	UPROPERTY(Transient)
	const UTetherPhysicsSolverLinear* CurrentLinearSolver = nullptr;
	
	UPROPERTY(Transient)
	const UTetherPhysicsSolverAngular* CurrentAngularSolver = nullptr;
	
	UPROPERTY(Transient)
	const UTetherReplay* CurrentReplaySystem = nullptr;
	
public:
	UPROPERTY(Transient, DuplicateTransient)
	TArray<ATetherEditorShapeActor*> ShapeActors;

	UPROPERTY()
	bool bHasWorldBegunPlay = false;

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
};
