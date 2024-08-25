// Copyright (c) Jared Taylor. All Rights Reserved


#include "TetherSpatialHashing.h"

#include "Animation/AnimInstanceProxy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherSpatialHashing)

void FTetherSpatialHashing::DrawDebugBucket(FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World,
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

void FTetherSpatialHashing::DrawDebugSpatialGrid(FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World, bool bDrawAllBuckets,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	if (!AnimInstanceProxy && !World)
	{
		return;
	}

	FIntVector MinBucketIndex(MAX_int32, MAX_int32, MAX_int32);
	FIntVector MaxBucketIndex(MIN_int32, MIN_int32, MIN_int32);

	// If we want to draw all buckets, first calculate the bounds
	if (bDrawAllBuckets)
	{
		for (const auto& HashEntry : SpatialHashMap)
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
					DrawDebugBucket(AnimInstanceProxy, World, BucketIndex, Input.BucketSize, Color, bPersistentLines, LifeTime, Thickness);
				}
			}
		}
	}
	else
	{
		// Draw only the buckets that contain shapes
		for (const auto& HashEntry : SpatialHashMap)
		{
			DrawDebugBucket(AnimInstanceProxy, World, HashEntry.Key, Input.BucketSize, Color, bPersistentLines, LifeTime, Thickness);
		}
	}
#endif
}
