// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Hashing/TetherHashingSpatial.h"

#include "TetherStatics.h"
#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherHashingSpatial)

namespace FTether
{
	TAutoConsoleVariable<bool> CVarTetherLogSpatialHashingBucketSize(TEXT("p.Tether.SpatialHashing.BucketSize.Log"), false, TEXT("Print Tether Spatial Hashing automatic resizing changes to output log"));
	TAutoConsoleVariable<bool> CVarTetherLogSpatialHashing(TEXT("p.Tether.SpatialHashing.Log"), false, TEXT("Log Tether Spatial Hashing"));
	TAutoConsoleVariable<bool> CVarTetherDrawSpatialHashingGrid(TEXT("p.Tether.SpatialHashing.Grid.Draw"), false, TEXT("Draw Tether Spatial Hashing Grid to world"));
}

void UTetherHashingSpatial::Solve(const FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform,
	float DeltaTime) const
{
	// Generate shape pairs based on spatial hashing and efficiency rating

	const auto* Input = InputData->GetDataIO<FSpatialHashingInput>();
	auto* Output = OutputData->GetDataIO<FSpatialHashingOutput>();
	
	// Clear previous pairs and hash map
	Output->ShapePairs.Reset();
	Output->SpatialHashMap.Reset();

	// Initialize the resulting bucket size. The condition isn't needed but may aid debugging
	Output->BucketSize = Input->BucketSizeMode != ETetherBucketSizingStrategy::Automatic ? Input->BucketSize : FVector::ZeroVector;
	
	// Establish a global minimum bucket size
	constexpr float GlobalMinBucketSize = KINDA_SMALL_NUMBER;  // Set this as a reasonable lower limit for the bucket size

	// First, determine the largest shape's bounding box
	FVector MaxBucketSize { GlobalMinBucketSize };  // Start with a global minimum

	// Loop through all shapes to find the maximum required bucket size
	const TArray<FTetherShape*> Shapes = *Input->Shapes;
	for (int32 i = 0; i < Input->Shapes->Num(); i++)
	{
		// Get the bounding box for the current shape in world space
		FTetherShape_AxisAlignedBoundingBox AABB = Shapes[i]->GetTetherShapeObject()->GetBoundingBox(*Shapes[i]);

		ensure(AABB.IsWorldSpace());

		// Calculate the AABB size in world space
		FVector AABBSize = AABB.Max - AABB.Min;

		// Compare with the current max bucket size and adjust accordingly
		MaxBucketSize.X = FMath::Max(MaxBucketSize.X, AABBSize.X);
		MaxBucketSize.Y = FMath::Max(MaxBucketSize.Y, AABBSize.Y);
		MaxBucketSize.Z = FMath::Max(MaxBucketSize.Z, AABBSize.Z);
	}

	// Set the bucket size globally to the largest found shape
	Output->BucketSize = MaxBucketSize;

	if (FTether::CVarTetherLogSpatialHashingBucketSize.GetValueOnAnyThread())
	{
		UE_LOG(LogTether, Log, TEXT("Final bucket size based on largest shape: (%f, %f, %f)"),
			Output->BucketSize.X, Output->BucketSize.Y, Output->BucketSize.Z);
	}

	// Now add all shapes to the spatial hash map using the fixed bucket size
	for (int32 i = 0; i < Input->Shapes->Num(); i++)
	{
		// Add shape to spatial hash
		FString DebugString = FString::Printf(TEXT("{ %s }"), *Shapes[i]->GetTetherShapeObject()->GetShapeDebugString());
		AddShapeToSpatialHash(Input, Output, i, Shapes[i], DebugString);

		// Output debug info
		if (FTether::CVarTetherLogSpatialHashing.GetValueOnAnyThread())
		{
			UE_LOG(LogTether, Log, TEXT("%s"), *DebugString);
		}
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

				const FTetherShape* ShapeA = Shapes[IndexA];
				const FTetherShape* ShapeB = Shapes[IndexB];

				// Check if shapes are in the same or adjacent buckets
				if (IsInSameOrAdjacentBucket(ShapeA, ShapeB))
				{
					// Determine which shape should perform the evaluation based on EfficiencyRating
					if (ShapeA->EfficiencyRating >= ShapeB->EfficiencyRating)
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

void UTetherHashingSpatial::AddShapeToSpatialHash(const FSpatialHashingInput* Input, FSpatialHashingOutput* Output,
	int32 ShapeIndex, const FTetherShape* Shape, FString& DebugString)
{
	FIntVector HashKey = ComputeSpatialHashKey(Input, Output, Shape, DebugString);
	DebugString += FString::Printf(TEXT(" HashKey: %s"), *HashKey.ToString());
	TArray<int32>& HashValue = Output->SpatialHashMap.FindOrAdd(HashKey);
	HashValue.Add(ShapeIndex);
}

FIntVector UTetherHashingSpatial::ComputeSpatialHashKey(const FSpatialHashingInput* Input, const FSpatialHashingOutput* Output,
	const FTetherShape* Shape, FString& DebugString)
{
	// Get the position of the shape's center
	FVector Position = Shape->LocalSpaceData->GetCenter();

	// Calculate the hash key based on the bucket size and position relative to the new origin
	return FIntVector(
		FMath::FloorToInt((Position.X) / Output->BucketSize.X),
		FMath::FloorToInt((Position.Y) / Output->BucketSize.Y),
		FMath::FloorToInt((Position.Z) / Output->BucketSize.Z)
	);
}

bool UTetherHashingSpatial::AreBucketsAdjacent(int32 BucketA, int32 BucketB)
{
	// Implement logic to determine if two buckets are adjacent
	// This could involve a simple comparison of bucket indices or more complex adjacency logic

	// DEVNOTE: This is Simple 1D Adjacency, but I'm not convinced 3D would actually perform better
	return FMath::Abs(BucketA - BucketB) <= 1;
}

bool UTetherHashingSpatial::IsInSameOrAdjacentBucket(const FTetherShape* ShapeA, const FTetherShape* ShapeB)
{
	// Implement logic to determine if two shapes are in the same or adjacent buckets
	// This could involve checking spatial hash values or bucket coordinates
	return ShapeA->HashIndex == ShapeB->HashIndex || AreBucketsAdjacent(ShapeA->HashIndex, ShapeB->HashIndex);
}

void UTetherHashingSpatial::DrawDebugBucket(FAnimInstanceProxy* Proxy, const UWorld* World,
	const FTransform& Transform, const FIntVector& BucketIndex, const FVector& BucketSize, const FColor& Color,
	bool bPersistentLines, float LifeTime, float Thickness)
{
#if ENABLE_DRAW_DEBUG
	FVector BucketMin = FVector(BucketIndex) * BucketSize;
	FVector BucketMax = BucketMin + BucketSize;

	BucketMin = Transform.TransformPosition(BucketMin);
	BucketMax = Transform.TransformPosition(BucketMax);

	// Calculate the center and extent of the bucket
	FVector BucketCenter = (BucketMin + BucketMax) * 0.5f;
	FVector BucketExtent = (BucketMax - BucketMin) * 0.5f;

	// Draw the box
	UTetherStatics::DrawBox(World, Proxy, BucketCenter, BucketExtent, FQuat::Identity, Color, bPersistentLines, LifeTime, Thickness);
#endif
}

void UTetherHashingSpatial::DrawDebug(const FSpatialHashingInput* Input, const FSpatialHashingOutput* Output,
	const FTransform& Transform, TArray<FTetherDebugText>* PendingDebugText, float LifeTime,
	 FAnimInstanceProxy* Proxy, const UWorld* World, bool bDrawAll, const FColor& Color,
	 bool bPersistentLines, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	if (!Proxy && !World)
	{
		return;
	}

	if (!FTether::CVarTetherDrawSpatialHashingGrid.GetValueOnAnyThread())
	{
		return;
	}

	// Apply the OriginOffset to the transform
	const FVector OriginOffset = Transform.TransformPosition(Input->OriginOffset);
	FTransform Origin = Transform;
	Origin.SetLocation(OriginOffset);

	// Draw a box at the center representing the origin of the spatial grid
	
	// Calculate the min and max points of the box
	FVector BucketMin = Origin.GetLocation() - Output->BucketSize * 0.5f;
	FVector BucketMax = Origin.GetLocation() + Output->BucketSize * 0.5f;
	
	const float OriginThickness = Thickness * 1.1f;
	const FColor OriginColor = FColor::Black;
	
	// Calculate the center and extent of the box
	FVector BoxCenter = (BucketMin + BucketMax) * 0.5f;
	FVector BoxExtent = (BucketMax - BucketMin) * 0.5f;

	// Use the UTetherStatics::DrawBox function to draw the box using the animation proxy
	UTetherStatics::DrawBox(World, Proxy, BoxCenter, BoxExtent, FQuat::Identity, OriginColor, bPersistentLines, LifeTime, OriginThickness);

	// Abort if we hashed nothing
	if (Output->SpatialHashMap.Num() == 0)
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
					DrawDebugBucket(Proxy, World, Origin, BucketIndex, Output->BucketSize, Color, bPersistentLines,
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
			DrawDebugBucket(Proxy, World, Origin, HashEntry.Key, Output->BucketSize, Color, bPersistentLines, 
			LifeTime, Thickness);
		}
	}
#endif
}
