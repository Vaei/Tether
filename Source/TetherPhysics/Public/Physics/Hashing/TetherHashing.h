// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherHashing.generated.h"

/**
 * Pair shapes together to determine collision testing order, or if they are tested at all
 */
UCLASS(Abstract, NotBlueprintable)
class TETHERPHYSICS_API UTetherHashing : public UObject
{
	GENERATED_BODY()

public:
	virtual void Solve(FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const {}
};
