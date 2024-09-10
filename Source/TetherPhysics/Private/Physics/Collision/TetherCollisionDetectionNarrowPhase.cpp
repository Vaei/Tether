// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Collision/TetherCollisionDetectionNarrowPhase.h"

#include "TetherIO.h"
#include "TetherStatics.h"
#include "Physics/Collision/TetherCollisionDetectionHandler.h"
#include "System/TetherDrawing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherCollisionDetectionNarrowPhase)

namespace FTether
{
	TAutoConsoleVariable<bool> CVarTetherLogNarrowPhaseCollision(TEXT("p.Tether.NarrowPhase.Log"), false, TEXT("Log Tether Narrow-Phase collisions"));

#if ENABLE_DRAW_DEBUG
	TAutoConsoleVariable<bool> CVarTetherDrawNarrowPhaseCollision(TEXT("p.Tether.NarrowPhase.Draw"), false, TEXT("Draw Tether Narrow-Phase collisions"));
#endif
}

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

void UTetherCollisionDetectionNarrowPhase::DrawDebug(const FTetherIO* InputData,
	const FTetherIO* OutputData, TArray<FTetherDebugText>* PendingDebugText, float LifeTime, FAnimInstanceProxy* Proxy, const
	UWorld* World, const FColor& CollisionColor, const FColor& NoCollisionColor, const FColor& InfoColor,
	const FColor& TextColor, bool bPersistentLines, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	if (!FTether::CVarTetherDrawNarrowPhaseCollision.GetValueOnAnyThread())
	{
		return;
	}
	
	const auto* Input = InputData->GetDataIO<FTetherNarrowPhaseCollisionInput>();
	const auto* Output = OutputData->GetDataIO<FTetherNarrowPhaseCollisionOutput>();

	for (const FTetherShape* Shape : *Input->Shapes)
	{
		const FTetherNarrowPhaseCollisionEntry* ShapeEntry = Output->Collisions.FindByPredicate(
			[Shape](const FTetherNarrowPhaseCollisionEntry& Entry)
		{
			return Entry.ShapeA == Shape || Entry.ShapeB == Shape;
		});

		const bool bColliding = ShapeEntry != nullptr;
		const FColor& Color = bColliding ? CollisionColor : NoCollisionColor;

		Shape->DrawDebug(World, Proxy, Color, bPersistentLines, LifeTime, Thickness);

		if (bColliding)
		{
			// Draw the contact point
			UTetherDrawing::DrawPoint(World, Proxy, ShapeEntry->ContactPoint, InfoColor, 10.f, bPersistentLines, LifeTime);

			// Draw penetration depth information as text
			FVector EndPoint = ShapeEntry->ContactPoint + ShapeEntry->ContactNormal * ShapeEntry->PenetrationDepth;

			const FString PenetrationDepthText = FString::Printf(TEXT("Penetration Depth: %s"), *FString::SanitizeFloat(ShapeEntry->PenetrationDepth, 2));
			UTetherDrawing::DrawText(PenetrationDepthText, PendingDebugText, Shape, EndPoint, TextColor);

			FVector EndPointOffset = EndPoint + FVector::UpVector * 2.5f;
			const FString RelativeVelocityText = FString::Printf(TEXT("Relative Velocity: %s"), *FString::SanitizeFloat(ShapeEntry->RelativeVelocity.Size(), 2));
			UTetherDrawing::DrawText(RelativeVelocityText, PendingDebugText, Shape, EndPointOffset, TextColor);

			// Optionally, draw the contact normal as a vector
			UTetherDrawing::DrawArrow(World, Proxy, ShapeEntry->ContactPoint, EndPoint, InfoColor, 10.f, bPersistentLines, LifeTime, Thickness);
		}
	}
#endif
}