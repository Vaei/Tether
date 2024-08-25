// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "TetherPhysicsSolver.h"
#include "TetherPhysicsSolverLinear.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TETHERPHYSICS_API UTetherPhysicsSolverLinear : public UTetherPhysicsSolver
{
	GENERATED_BODY()

public:
	virtual void Solve(const FLinearInput& Input, FLinearOutput& Output, const FTransform& Transform, float DeltaTime) const;
};
