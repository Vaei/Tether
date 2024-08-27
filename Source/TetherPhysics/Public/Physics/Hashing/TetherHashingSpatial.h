// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherHashing.h"
#include "TetherHashingSpatial.generated.h"


/**
 * Pair shapes by checking if they are either in the same bucket or a neighbouring bucket
 * This allows us to bypass overlap testing for shapes that aren't close enough to potentially collide
 */
UCLASS(Abstract, NotBlueprintable)
class TETHERPHYSICS_API UTetherHashingSpatial : public UTetherHashing
{
	GENERATED_BODY()

public:
	virtual void Solve(FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const override;

protected:
	// Function to add shapes to the spatial hash map
	static void AddShapeToSpatialHash(const FSpatialHashingInput* Input, FSpatialHashingOutput* Output,
		int32 ShapeIndex, const FTetherShape& Shape, const FTransform& WorldOrigin)
	{
		FIntVector HashKey = ComputeSpatialHashKey(Input, Shape, WorldOrigin);
		TArray<int32>& Value = Output->SpatialHashMap.FindOrAdd(HashKey);
		Value.Add(ShapeIndex);
	}

	/** Compute the spatial hash key for a given shape */
	static FIntVector ComputeSpatialHashKey(const FSpatialHashingInput* Input, const FTetherShape& Shape, const FTransform& WorldOrigin)
	{
		// Rotate Input.Origin by the rotation of WorldOrigin and then translate it
		FVector Origin = WorldOrigin.TransformPosition(Input->Origin);
    
		// Get the position of the shape's center
		FVector Position = Shape.GetCenter();

		// Calculate the hash key based on the bucket size and position relative to the new origin
		return FIntVector(
			FMath::FloorToInt((Position.X - Origin.X) / Input->BucketSize.X),
			FMath::FloorToInt((Position.Y - Origin.Y) / Input->BucketSize.Y),
			FMath::FloorToInt((Position.Z - Origin.Z) / Input->BucketSize.Z)
		);
	}
	
	static bool AreBucketsAdjacent(int32 BucketA, int32 BucketB)
	{
		// Implement logic to determine if two buckets are adjacent
		// This could involve a simple comparison of bucket indices or more complex adjacency logic

		// DEVNOTE: This is Simple 1D Adjacency, but I'm not convinced 3D would actually perform better
		return FMath::Abs(BucketA - BucketB) <= 1;
	}

	static bool IsInSameOrAdjacentBucket(const FTetherShape& ShapeA, const FTetherShape& ShapeB)
	{
		// Implement logic to determine if two shapes are in the same or adjacent buckets
		// This could involve checking spatial hash values or bucket coordinates
		return ShapeA.Bucket == ShapeB.Bucket || AreBucketsAdjacent(ShapeA.Bucket, ShapeB.Bucket);
	}

public:
	static void DrawDebugBucket(FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World,
		const FIntVector& BucketIndex, const FVector& BucketSize, const FColor& Color = FColor::Green,
		bool bPersistentLines = false, float LifeTime = -1.0f, float Thickness = 1.0f);
	
	virtual void DrawDebug(const FSpatialHashingInput* Input, const FSpatialHashingOutput* Output,
		FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World, bool bDrawAll, const FColor& Color,
		bool bPersistentLines, float LifeTime, float Thickness) const override;
};
