// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameplayTask.h"

#include "TetherPhysicsTask.generated.h"

/**
 * @TODO
 * Updates on GameThread
 */
UCLASS()
class TETHER_API UTetherPhysicsTask : public UGameplayTask
{
	GENERATED_BODY()

public:
	// UTetherPhysicsTask(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// static UTetherPhysicsTask* TetherPhysicsTask(FGameplayTag LinearSolver);
};