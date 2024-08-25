// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shapes/TetherShape.h"
#include "UObject/Object.h"
#include "TetherCollisionDetectionNarrowPhase.generated.h"

struct FTetherNarrowPhaseCollisionOutput;
/**
 * 
 */
UCLASS()
class TETHER_API UTetherCollisionDetectionNarrowPhase : public UObject
{
	GENERATED_BODY()

public:
	virtual void DetectCollision(const TArray<FTetherShape>& BoneShapes, TArray<FTetherNarrowPhaseCollisionOutput>& Output) const;
	virtual void DrawDebug(const TArray<FTetherNarrowPhaseCollisionOutput>& CollisionOutput, FAnimInstanceProxy* AnimInstanceProxy) const;
};
