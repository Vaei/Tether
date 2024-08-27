// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "TetherPhysicsSolver.h"
#include "TetherPhysicsSolverAngular.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherPhysicsSolverAngular : public UTetherPhysicsSolver
{
	GENERATED_BODY()

public:
	virtual void Solve(FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const override;
};
