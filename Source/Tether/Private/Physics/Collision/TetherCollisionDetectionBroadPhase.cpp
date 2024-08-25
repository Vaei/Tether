// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "Physics/Collision/TetherCollisionDetectionBroadPhase.h"

#include "TetherIO.h"
#include "TetherStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherCollisionDetectionBroadPhase)

namespace FTether
{
	TAutoConsoleVariable<bool> CVarTetherLogBroadPhaseCollision(TEXT("a.Tether.LogBroadPhaseCollision"), false, TEXT("Log Tether Broad-Phase collisions"));
	TAutoConsoleVariable<bool> CVarTetherDrawBroadPhaseCollision(TEXT("a.Tether.DrawBroadPhaseCollision"), false, TEXT("Draw Tether Broad-Phase collisions"));
}

void UTetherCollisionDetectionBroadPhase::DetectCollision(const TArray<FTetherShape>& BoneShapes, FTetherBroadPhaseCollisionOutput& Output) const
{
	// Output.CollisionPairings.Reset();
	//
	// // Loop through all pairs of bone shapes
	// for (int32 i = 0; i < BoneShapes.Num(); ++i)
	// {
	// 	for (int32 j = i + 1; j < BoneShapes.Num(); ++j)
	// 	{
	// 		const FTetherShape& ShapeA = BoneShapes[i];
	// 		const FTetherShape& ShapeB = BoneShapes[j];
	//
	// 		// Check for overlap
	// 		if (ShapeA.IsValid() && ShapeB.IsValid() && ShapeA.CheckOverlap(ShapeB))
	// 		{
	// 			// Skip collision based on ignored TYPE
	// 			if (FTetherShape::AreShapesIgnoringEachOther(ShapeA, ShapeB))
	// 			{
	// 				continue;
	// 			}
	//
	// 			if (FTether::CVarTetherLogBroadPhaseCollision.GetValueOnAnyThread())
	// 			{
	// 				UE_LOG(LogTether, Log, TEXT("Potential broad-phase collision detected between Shape %d and Shape %d."), i, j);
	// 			}
	// 			
	// 			// Add the collision pair to the output
	// 			Output.CollisionPairings.Add(FTetherBroadCollisionPair(i, j));
	// 		}
	// 	}
	// }
}

void UTetherCollisionDetectionBroadPhase::DrawDebug(const TArray<FTetherShape>& BoneShapes,
	const FTetherBroadPhaseCollisionOutput& CollisionOutput, FAnimInstanceProxy* AnimInstanceProxy, bool bForceDraw) const
{
// #if ENABLE_DRAW_DEBUG
// 	if (!bForceDraw || !FTether::CVarTetherDrawBroadPhaseCollision.GetValueOnAnyThread())
// 	{
// 		return;
// 	}
// 	
// 	// Draw all bounding volumes
// 	for (const FTetherShape& Shape : BoneShapes)
// 	{
// 		Shape.DrawDebug(AnimInstanceProxy, FColor::Blue); // Draw all shapes in blue by default
// 	}
//
// 	// Highlight potential collisions
// 	for (const FTetherBroadCollisionPair& Pair : CollisionOutput.CollisionPairings)
// 	{
// 		const FTetherShape& ShapeA = BoneShapes[Pair.FirstIndex];
// 		const FTetherShape& ShapeB = BoneShapes[Pair.SecondIndex];
//
// 		ShapeA.DrawDebug(AnimInstanceProxy, FColor::Yellow); // Highlight potential collision shapes in yellow
// 		ShapeB.DrawDebug(AnimInstanceProxy, FColor::Yellow);
// 	}
// #endif
}
