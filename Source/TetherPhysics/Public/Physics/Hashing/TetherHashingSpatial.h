// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherHashing.h"
#include "TetherHashingSpatial.generated.h"


/**
 * Spatial hashing system used in physics simulations.
 *
 * UTetherHashingSpatial provides a framework for pairing shapes together in a physics simulation
 * to determine the order of collision tests or to decide whether certain shapes should be tested for collisions at all.
 * 
 * Spatial hashing techniques help optimize collision detection by reducing the number of necessary collision checks,
 * especially in complex scenes with many interacting objects.
 * 
 * This class is designed to be extended with specific hashing algorithms that implement the collision pairing logic.
 * It also includes methods for debugging and visualizing the results of the spatial hashing process.
 * 
 * Key Responsibilities:
 * - Solve: Pair shapes for collision testing based on spatial proximity and other criteria.
 * - DrawDebug: Visualize the spatial hash grid and collision pairs for debugging and optimization.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherHashingSpatial : public UTetherHashing
{
	GENERATED_BODY()

public:
	virtual void Solve(const FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const override;

protected:
	// Function to add shapes to the spatial hash map
	static void AddShapeToSpatialHash(const FSpatialHashingInput* Input, FSpatialHashingOutput* Output,
		int32 ShapeIndex, const FTetherShape* Shape, const FTransform& WorldOrigin, FString& DebugString);

	/** Compute the spatial hash key for a given shape */
	static FIntVector ComputeSpatialHashKey(const FSpatialHashingInput* Input, const FSpatialHashingOutput* Output, const FTetherShape* Shape, const FTransform& WorldOrigin, FString& DebugString);

	static bool AreBucketsAdjacent(int32 BucketA, int32 BucketB);

	static bool IsInSameOrAdjacentBucket(const FTetherShape* ShapeA, const FTetherShape* ShapeB);

public:
	static void DrawDebugBucket(FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World, const FTransform& Transform,
		const FIntVector& BucketIndex, const FVector& BucketSize, const FColor& Color = FColor::Green,
		bool bPersistentLines = false, float LifeTime = -1.0f, float Thickness = 1.0f);
	
	virtual void DrawDebug(const FSpatialHashingInput* Input, const FSpatialHashingOutput* Output,
		const FTransform& Transform, FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World, bool bDrawAll,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
