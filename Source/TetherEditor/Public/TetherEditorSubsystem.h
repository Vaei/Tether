// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TetherIO.h"
#include "TetherPhysicsTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "System/TetherMessaging.h"
#include "Tether/Public/TetherPhysicsUpdate.h"
#include "TetherEditorSubsystem.generated.h"

class ATetherEditorShapeActor;
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
	const UTetherDataAsset* DataAsset;

protected:
	FTetherPhysicsUpdate PhysicsUpdate = {60.f};

	UPROPERTY(BlueprintReadOnly, Category=Tether)
	FTetherCommonSharedData SharedData;

	TMap<FTetherShape*, FTetherCommonShapeData*> ShapeData;

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
