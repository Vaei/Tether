// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Collision/TetherCollisionDetectionNarrowPhase.h"

#include "TetherIO.h"
#include "TetherStatics.h"
#include "Physics/Collision/TetherCollisionDetectionHandler.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherCollisionDetectionNarrowPhase)

namespace FTether
{
	TAutoConsoleVariable<bool> CVarTetherLogNarrowPhaseCollision(TEXT("p.Tether.NarrowPhase.Log"), false, TEXT("Log Tether Narrow-Phase collisions"));
}

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

void UTetherCollisionDetectionNarrowPhase::DetectCollision(const FTetherIO* InputData, FTetherIO* OutputData,
	const FTetherIO* LinearOutputData, FTetherIO* AngularOutputData,
	const UTetherCollisionDetectionHandler* CollisionDetectionHandler) const
{
	const auto* Input = InputData->GetDataIO<FTetherNarrowPhaseCollisionInput>();
	auto* Output = OutputData->GetDataIO<FTetherNarrowPhaseCollisionOutput>();

	const auto* LinearOutput = LinearOutputData->GetDataIO<FLinearOutput>();
	const auto* AngularOutput = AngularOutputData->GetDataIO<FAngularOutput>();

	// Clear the output before starting
	Output->Collisions.Reset();

	// Iterate through each collision pairing from the broad-phase
	for (const FTetherShapePair& Pair : *Input->CollisionPairings)
	{
		const FTetherShape* ShapeA = (*Input->Shapes)[Pair.ShapeIndexA];
		const FTetherShape* ShapeB = (*Input->Shapes)[Pair.ShapeIndexB];

		// Perform narrow-phase collision check between ShapeA and ShapeB
		FTetherNarrowPhaseCollisionEntry CollisionEntry { ShapeA, ShapeB };
		if (CollisionDetectionHandler->CheckNarrowCollision(ShapeA, ShapeB, CollisionEntry))
		{
			// Get Velocity at Point
			const FVector ContactVelocityA = UTetherStatics::GetVelocityAtPoint(CollisionEntry.ContactPoint,
				ShapeA->GetCenter(), LinearOutput->GetLinearVelocity(ShapeA), AngularOutput->GetAngularVelocity(ShapeA));
			
			const FVector ContactVelocityB = UTetherStatics::GetVelocityAtPoint(CollisionEntry.ContactPoint,
				ShapeB->GetCenter(), LinearOutput->GetLinearVelocity(ShapeB), AngularOutput->GetAngularVelocity(ShapeB));
			
			// Calculate relative velocity at the contact point
			CollisionEntry.RelativeVelocity = ContactVelocityA - ContactVelocityB;

			// Calculate the vector from the center of A to the center of B
			FVector CollisionVector = ShapeB->GetCenter() - ShapeA->GetCenter();
			
			// Normalize the collision vector to get the contact normal
			CollisionEntry.ContactNormal = CollisionVector.GetSafeNormal();

			// Add the collision entry to the output
			Output->Collisions.Add(CollisionEntry);

			// Debug logging
			if (FTether::CVarTetherLogNarrowPhaseCollision.GetValueOnAnyThread())
			{
				UE_LOG(LogTether, Warning, TEXT("[ %s ] Shape { %s } narrow-phase collision with { %s } at Contact Point { %s }, Penetration Depth: { %.3f }"),
					*FString(__FUNCTION__), *ShapeA->GetName(), *ShapeB->GetName(), *CollisionEntry.ContactPoint.ToString(), CollisionEntry.PenetrationDepth);
			}
		}
	}
}

void UTetherCollisionDetectionNarrowPhase::DrawDebug(const TArray<FTetherNarrowPhaseCollisionOutput>& CollisionOutputs,
	FAnimInstanceProxy* Proxy) const
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
