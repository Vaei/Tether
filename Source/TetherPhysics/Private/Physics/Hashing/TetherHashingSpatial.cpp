// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Hashing/TetherHashingSpatial.h"

#include "Animation/AnimInstanceProxy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherHashingSpatial)

void UTetherHashingSpatial::Solve(FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform,
	float DeltaTime) const
{
	// Generate shape pairs based on spatial hashing and efficiency rating

	const auto* Input = InputData->GetDataIO<FSpatialHashingInput>();
	auto* Output = OutputData->GetDataIO<FSpatialHashingOutput>();
	
	// Clear previous pairs and hash map
	Output->ShapePairs.Reset();
	Output->SpatialHashMap.Reset();

	// Add all shapes to the spatial hash map
	const TArray<FTetherShape>& Shapes = *Input->Shapes;
	
	for (int32 i = 0; i < Input->Shapes->Num(); i++)
	{
		AddShapeToSpatialHash(Input, Output, i, Shapes[i], Transform);
	}

	// Generate pairs based on spatial proximity and efficiency rating
	for (const auto& HashEntry : Output->SpatialHashMap)
	{
		const TArray<int32>& Indices = HashEntry.Value;

		for (int32 i = 0; i < Indices.Num(); i++)
		{
			for (int32 j = i + 1; j < Indices.Num(); j++)
			{
				int32 IndexA = Indices[i];
				int32 IndexB = Indices[j];

				const FTetherShape& ShapeA = Shapes[IndexA];
				const FTetherShape& ShapeB = Shapes[IndexB];

				// Check if shapes are in the same or adjacent buckets
				if (IsInSameOrAdjacentBucket(ShapeA, ShapeB))
				{
					// Determine which shape should perform the evaluation based on EfficiencyRating
					if (ShapeA.EfficiencyRating >= ShapeB.EfficiencyRating)
					{
						Output->ShapePairs.Add(FTetherShapePair(IndexA, IndexB));
					}
					else
					{
						Output->ShapePairs.Add(FTetherShapePair(IndexB, IndexA));
					}
				}
			}
		}
	}
}

void UTetherHashingSpatial::DrawDebugBucket(FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World,
	const FIntVector& BucketIndex, const FVector& BucketSize, const FColor& Color, bool bPersistentLines,
	float LifeTime, float Thickness)
{
	#if ENABLE_DRAW_DEBUG
	FVector BucketMin = FVector(BucketIndex) * BucketSize;
	FVector BucketMax = BucketMin + BucketSize;

	// Draw lines along the edges of the bucket to form a cube
	if (AnimInstanceProxy)
	{
		// Define the eight corners of the box
		FVector V0 = BucketMin;
		FVector V1 = FVector(BucketMax.X, BucketMin.Y, BucketMin.Z);
		FVector V2 = FVector(BucketMax.X, BucketMax.Y, BucketMin.Z);
		FVector V3 = FVector(BucketMin.X, BucketMax.Y, BucketMin.Z);
		FVector V4 = FVector(BucketMin.X, BucketMin.Y, BucketMax.Z);
		FVector V5 = FVector(BucketMax.X, BucketMin.Y, BucketMax.Z);
		FVector V6 = BucketMax;
		FVector V7 = FVector(BucketMin.X, BucketMax.Y, BucketMax.Z);
		
		// No AnimDrawDebugBox function exists
		AnimInstanceProxy->AnimDrawDebugLine(V0, V1, Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(V1, V2, Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(V2, V3, Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(V3, V0, Color, bPersistentLines, LifeTime, Thickness);

		AnimInstanceProxy->AnimDrawDebugLine(V4, V5, Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(V5, V6, Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(V6, V7, Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(V7, V4, Color, bPersistentLines, LifeTime, Thickness);

		AnimInstanceProxy->AnimDrawDebugLine(V0, V4, Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(V1, V5, Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(V2, V6, Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(V3, V7, Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)
	{
		DrawDebugBox(World, (BucketMin + BucketMax) * 0.5f, BucketSize * 0.5f, FQuat::Identity, Color, bPersistentLines, LifeTime, 0, Thickness);
	}
#endif
}

void UTetherHashingSpatial::DrawDebug(const FSpatialHashingInput* Input, const FSpatialHashingOutput* Output,
	FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World, bool bDrawAll, const FColor& Color,
	bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	if (!AnimInstanceProxy && !World)
	{
		return;
	}

	FIntVector MinBucketIndex(MAX_int32, MAX_int32, MAX_int32);
	FIntVector MaxBucketIndex(MIN_int32, MIN_int32, MIN_int32);

	// If we want to draw all buckets, first calculate the bounds
	if (bDrawAll)
	{
		for (const auto& HashEntry : Output->SpatialHashMap)
		{
			MinBucketIndex.X = FMath::Min(MinBucketIndex.X, HashEntry.Key.X);
			MinBucketIndex.Y = FMath::Min(MinBucketIndex.Y, HashEntry.Key.Y);
			MinBucketIndex.Z = FMath::Min(MinBucketIndex.Z, HashEntry.Key.Z);

			MaxBucketIndex.X = FMath::Max(MaxBucketIndex.X, HashEntry.Key.X);
			MaxBucketIndex.Y = FMath::Max(MaxBucketIndex.Y, HashEntry.Key.Y);
			MaxBucketIndex.Z = FMath::Max(MaxBucketIndex.Z, HashEntry.Key.Z);
		}

		// Iterate over all possible bucket indices in the range from MinBucketIndex to MaxBucketIndex
		for (int32 X = MinBucketIndex.X; X <= MaxBucketIndex.X; X++)
		{
			for (int32 Y = MinBucketIndex.Y; Y <= MaxBucketIndex.Y; Y++)
			{
				for (int32 Z = MinBucketIndex.Z; Z <= MaxBucketIndex.Z; Z++)
				{
					FIntVector BucketIndex(X, Y, Z);
					DrawDebugBucket(AnimInstanceProxy, World, BucketIndex, Input->BucketSize, Color, bPersistentLines,
						LifeTime, Thickness);
				}
			}
		}
	}
	else
	{
		// Draw only the buckets that contain shapes
		for (const auto& HashEntry : Output->SpatialHashMap)
		{
			DrawDebugBucket(AnimInstanceProxy, World, HashEntry.Key, Input->BucketSize, Color, bPersistentLines, 
			LifeTime, Thickness);
		}
	}
#endif
}
