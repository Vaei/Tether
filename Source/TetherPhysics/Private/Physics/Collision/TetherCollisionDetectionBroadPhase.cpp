// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Collision/TetherCollisionDetectionBroadPhase.h"

#include "TetherIO.h"
#include "TetherStatics.h"
#include "Physics/Collision/TetherCollisionDetectionHandler.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherCollisionDetectionBroadPhase)

namespace FTether
{
	TAutoConsoleVariable<bool> CVarTetherLogBroadPhaseCollision(TEXT("p.Tether.BroadPhase.Log"), false, TEXT("Log Tether Broad-Phase collisions"));

#if ENABLE_DRAW_DEBUG
	TAutoConsoleVariable<bool> CVarTetherDrawBroadPhaseCollision(TEXT("p.Tether.BroadPhase.Draw"), false, TEXT("Draw Tether Broad-Phase collisions"));
#endif
}

void UTetherCollisionDetectionBroadPhase::DetectCollision(const FTetherIO* InputData, FTetherIO* OutputData,
	const UTetherCollisionDetectionHandler* CollisionDetectionHandler, float DeltaTime, double WorldTime) const
{
	const auto* Input = InputData->GetDataIO<FBroadPhaseInput>();
	auto* Output = OutputData->GetDataIO<FBroadPhaseOutput>();

	// Clear the output before starting
	Output->CollisionPairings.Reset();

	// Iterate through each potential collision pair
	for (const FTetherShapePair& Pair : *Input->PotentialCollisionPairings)
	{
		// Perform broad-phase collision checks between ShapeA and ShapeB
		if (CollisionDetectionHandler->CheckBroadCollision(Pair.ShapeA, Pair.ShapeB))
		{
			// If a collision is detected, add it to the output
			Output->CollisionPairings.Add(Pair);

			// Assign WorldTime so we know when they might need to be woken in later stages
			Pair.ShapeA->LastBroadCollisionTime = WorldTime;
			Pair.ShapeB->LastBroadCollisionTime = WorldTime;

			// Debug logging
			if (FTether::CVarTetherLogBroadPhaseCollision.GetValueOnAnyThread())
			{
				UE_LOG(LogTether, Warning, TEXT("[ %s ] Shape { %s } broad-phase overlap with { %s }"),
					*FString(__FUNCTION__), *Pair.ShapeA->GetName(), *Pair.ShapeB->GetName());
			}
		}
	}
}

void UTetherCollisionDetectionBroadPhase::DrawDebug(const TArray<FTetherShape*>* Shapes, const FTetherIO* InputData,
	const FTetherIO* OutputData, TArray<FTetherDebugText>* PendingDebugText, float LifeTime, FAnimInstanceProxy* Proxy,
	UWorld* World, bool bForceDraw, const FColor& NoTestColor, const FColor& OverlapColor, const FColor& NoOverlapColor) const
{
#if ENABLE_DRAW_DEBUG
	if (!bForceDraw && !FTether::CVarTetherDrawBroadPhaseCollision.GetValueOnAnyThread())
	{
		return;
	}
	
	if (!Proxy && !World)
	{
		return;
	}
	
	const auto* Input = InputData->GetDataIO<FBroadPhaseInput>();
	const auto* Output = OutputData->GetDataIO<FBroadPhaseOutput>();

	// Draw bounding boxes - this is O(n^2) at least, but it isn't in shipping builds
	for (const FTetherShape* Shape : *Shapes)
	{
		bool bFoundInCollisionPairings = false;
		bool bFoundInPotentialCollisionPairings = false;

		// Check in Output->CollisionPairings - did it overlap something?
		for (const FTetherShapePair& Pair : Output->CollisionPairings)
		{
			if (Pair.ContainsShape(Shape))
			{
				bFoundInCollisionPairings = true;
				break;  // Stop searching once found
			}
		}

		// Check in Input->PotentialCollisionPairings if it wasn't found in Output->CollisionPairings
		if (!bFoundInCollisionPairings)
		{
			for (const FTetherShapePair& Pair : *Input->PotentialCollisionPairings)
			{
				if (Pair.ContainsShape(Shape))
				{
					bFoundInPotentialCollisionPairings = true;
					break;  // Stop searching once found
				}
			}
		}

		// Now decide which color to draw it as based on where the shape was found
		FColor DebugColor;
		if (bFoundInCollisionPairings)
		{
			// Overlapped
			DebugColor = OverlapColor;
		}
		else if (bFoundInPotentialCollisionPairings)
		{
			// No overlap
			DebugColor = NoOverlapColor;
		}
		else
		{
			// Shape wasn't found in either array, wasn't tested
			DebugColor = NoTestColor;
		}
		
		FTetherShape_AxisAlignedBoundingBox AABB = Shape->GetTetherShapeObject()->GetBoundingBox(*Shape);
		AABB.DrawDebug(World, Proxy, DebugColor, false, LifeTime, 1.f);
	}
#endif
}
