// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TetherIO.h"
#include "Subsystems/WorldSubsystem.h"
#include "System/TetherMessaging.h"
#include "Tether/Public/TetherPhysicsUpdate.h"
#include "TetherEditorSubsystem.generated.h"

class UTetherActivityStateHandler;
class UTetherIntegrationSolver;
class ATetherEditorShapeActor;
class UTetherReplay;
class UTetherPhysicsSolverAngular;
class UTetherPhysicsSolverLinear;
class UTetherCollisionDetectionNarrowPhase;
class UTetherCollisionDetectionBroadPhase;
class UTetherCollisionDetectionHandler;
class UTetherHashing;
class UTetherDataAsset;

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
	
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	FSpatialHashingInput SpatialHashingInput;

	UPROPERTY(BlueprintReadOnly, Category=Tether)
	const UTetherDataAsset* Data;

protected:
	FTetherPhysicsUpdate PhysicsUpdate = { 60.f };

	UPROPERTY()
	FSpatialHashingOutput SpatialHashingOutput;

	UPROPERTY()
	FTetherActivityStateInput ActivityStateHandlerInput;
	
	UPROPERTY()
	FLinearInput LinearInput;

	UPROPERTY()
	FAngularInput AngularInput;

	UPROPERTY()
	FLinearOutput LinearOutput;

	UPROPERTY()
	FAngularOutput AngularOutput;

	UPROPERTY()
	FIntegrationInput IntegrationInput;

	UPROPERTY()
	FIntegrationOutput IntegrationOutput;

	UPROPERTY()
	FRecordedPhysicsData RecordedData;
	
	UPROPERTY()
	FTetherBroadPhaseCollisionInput BroadPhaseInput;

	UPROPERTY()
	FTetherBroadPhaseCollisionOutput BroadPhaseOutput;

	UPROPERTY()
	FTetherNarrowPhaseCollisionInput NarrowPhaseInput;

	UPROPERTY()
	FTetherNarrowPhaseCollisionOutput NarrowPhaseOutput;

	UPROPERTY(Transient)
	FGameplayTag LastHashingSystem = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastCollisionDetectionHandler = FGameplayTag::EmptyTag;

	UPROPERTY(Transient)
	FGameplayTag LastBroadPhaseCollisionDetection = FGameplayTag::EmptyTag;
	
	UPROPERTY(Transient)
	FGameplayTag LastNarrowPhaseCollisionDetection = FGameplayTag::EmptyTag;

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
	const UTetherHashing* CurrentHashingSystem = nullptr;
		
	UPROPERTY(Transient)
	const UTetherCollisionDetectionHandler* CurrentCollisionDetectionHandler = nullptr;
	
	UPROPERTY(Transient)
	const UTetherCollisionDetectionBroadPhase* CurrentBroadPhaseCollisionDetection = nullptr;

	UPROPERTY(Transient)
	const UTetherCollisionDetectionNarrowPhase* CurrentNarrowPhaseCollisionDetection = nullptr;

	UPROPERTY(Transient)
	const UTetherActivityStateHandler* CurrentActivityStateHandler = nullptr;
	
	UPROPERTY(Transient)
	const UTetherPhysicsSolverLinear* CurrentLinearSolver = nullptr;
	
	UPROPERTY(Transient)
	const UTetherPhysicsSolverAngular* CurrentAngularSolver = nullptr;

	UPROPERTY(Transient)
	const UTetherIntegrationSolver* CurrentIntegrationSolver = nullptr;
	
	UPROPERTY(Transient)
	const UTetherReplay* CurrentReplaySystem = nullptr;
	
public:
	UPROPERTY(Transient, DuplicateTransient)
	TArray<ATetherEditorShapeActor*> ShapeActors;

	UPROPERTY()
	bool bHasWorldBegunPlay = false;

protected:
	/** FMessageLog and UE_LOG handler - Primarily exists to avoid printing unique messages repeatedly on tick */
	UPROPERTY(Transient)
	FTetherMessageLog MessageLog;

	FDelegateHandle GDebugDrawHandleGame;
	FDelegateHandle GDebugDrawHandleEditor;

	UPROPERTY()
	FTetherDebugTextService DebugTextService;

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	virtual bool UpdateGameplayTagReferences();
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
};
