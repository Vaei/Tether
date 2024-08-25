// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shapes/TetherShape.h"
#include "UObject/Object.h"
#include "TetherCollisionDetectionBroadPhase.generated.h"

struct FTetherBroadPhaseCollisionOutput;
/**
 * 
 */
UCLASS()
class TETHERPHYSICS_API UTetherCollisionDetectionBroadPhase : public UObject
{
	GENERATED_BODY()

public:
	virtual void DetectCollision(const TArray<FTetherShape>& BoneShapes, FTetherBroadPhaseCollisionOutput& Output) const;
	virtual void DrawDebug(const TArray<FTetherShape>& BoneShapes, const FTetherBroadPhaseCollisionOutput& CollisionOutput, FAnimInstanceProxy* AnimInstanceProxy, bool bForceDraw = false) const;
};
