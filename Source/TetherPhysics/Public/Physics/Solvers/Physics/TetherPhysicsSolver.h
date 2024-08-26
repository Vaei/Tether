// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherPhysicsSolver.generated.h"

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class TETHERPHYSICS_API UTetherPhysicsSolver : public UObject
{
	GENERATED_BODY()

public:
	virtual void Solve(FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const {}
};
