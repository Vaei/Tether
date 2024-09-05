// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Collision/TetherCollisionDetectionNarrowPhase.h"

#include "TetherIO.h"
#include "TetherStatics.h"
#include "Animation/AnimInstanceProxy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherCollisionDetectionNarrowPhase)

namespace FTether
{
	TAutoConsoleVariable<bool> CVarTetherLogNarrowPhaseCollision(TEXT("p.Tether.NarrowPhase.Log"), false, TEXT("Log Tether Narrow-Phase collisions"));
}

void UTetherCollisionDetectionNarrowPhase::DetectCollision(const TArray<FTetherShape>& BoneShapes,
	TArray<FTetherNarrowPhaseCollisionOutput>& Output) const
{
	// Output.Reset();
	//
	// // Loop through all pairs of shapes in the array
	// for (int32 i = 0; i < BoneShapes.Num(); ++i)
	// {
	// 	for (int32 j = i + 1; j < BoneShapes.Num(); ++j)
	// 	{
	// 		const FTetherShape& ShapeA = BoneShapes[i];
	// 		const FTetherShape& ShapeB = BoneShapes[j];
	//
	// 		if (ShapeA.IsValid() && ShapeB.IsValid())
	// 		{
	// 			if (FTetherShape::AreShapesIgnoringEachOther(ShapeA, ShapeB))
	// 			{
	// 				continue;
	// 			}
	//
	// 			// Perform the detailed collision check between ShapeA and ShapeB
	// 			FTetherNarrowPhaseCollisionOutput CollisionResult;
	// 			if (ShapeA.CheckNarrowCollision(ShapeB, CollisionResult))
	// 			{
	// 				if (FTether::CVarTetherLogNarrowPhaseCollision.GetValueOnAnyThread())
	// 				{
	// 					UE_LOG(LogTether, Log, TEXT("Narrow-Phase Collision detected between Shape %d and Shape %d!"), i, j);
	// 					UE_LOG(LogTether, Log, TEXT("Contact Point: %s"), *CollisionResult.ContactPoint.ToString());
	// 					UE_LOG(LogTether, Log, TEXT("Penetration Depth: %f"), CollisionResult.PenetrationDepth);
	// 				}
	//
	// 				// Store the shapes, primarily for debugging at this point
	// 				CollisionResult.ShapeA = ShapeA;
	// 				CollisionResult.ShapeB = ShapeB;
	//
	// 				// Store the collision result
	// 				Output.Add(CollisionResult);
	// 			}
	// 		}
	// 	}
	// }
}

void UTetherCollisionDetectionNarrowPhase::DrawDebug(const TArray<FTetherNarrowPhaseCollisionOutput>& CollisionOutputs,
	FAnimInstanceProxy* AnimInstanceProxy) const
{
	// for (const FTetherNarrowPhaseCollisionOutput& CollisionOutput : CollisionOutputs)
	// {
	// 	if (CollisionOutput.bHasCollision)
	// 	{
	// 		// Highlight actual collisions in a different color (e.g., red)
	// 		CollisionOutput.ShapeA.DrawDebug(AnimInstanceProxy, FColor::Red);
	// 		CollisionOutput.ShapeB.DrawDebug(AnimInstanceProxy, FColor::Red);
	//
	// 		// Draw the collision point
	// 		AnimInstanceProxy->AnimDrawDebugPoint(CollisionOutput.ContactPoint, 10.0f, FColor::Red);
	//
	// 		// Optionally, draw the penetration depth
	// 		FVector PenetrationEnd = CollisionOutput.ContactPoint + FVector(0, 0, CollisionOutput.PenetrationDepth);
	// 		AnimInstanceProxy->AnimDrawDebugLine(CollisionOutput.ContactPoint, PenetrationEnd, FColor::Red, false, -1.f, 2.f, SDPG_World);
	// 	}
	// }
}
