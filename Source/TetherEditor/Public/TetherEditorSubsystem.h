// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TetherEditorSubsystem.generated.h"

class ATetherEditorShapeActor;
/**
 * 
 */
UCLASS()
class TETHEREDITOR_API UTetherEditorSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, DuplicateTransient)
	TArray<ATetherEditorShapeActor*> ShapeActors;
	
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
};
