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
	/**
	 * Implements the core logic for pairing or grouping objects/data based on a specific hashing algorithm.
	 * 
	 * @param InputData  Pointer to the input data containing objects or data to be hashed.
	 * @param OutputData Pointer to the output data where the hashing results will be stored.
	 * @param Origin	 The center of the spatial hashing grid to be applied during the hashing process, if applicable.
	 * @param DeltaTime  The time step for the simulation, used for time-dependent hashing calculations, if applicable.
	 * @param WorldTime	 Current WorldTime appended by TimeTicks
	 */
	virtual void Solve(const TArray<FTetherShape*>* Shapes, const FTetherIO* InputData, FTetherIO* OutputData,
		const FTransform& Origin, float DeltaTime, double WorldTime) const override;

protected:
	// Function to add shapes to the spatial hash map
	static void AddShapeToSpatialHash(const FSpatialHashingInput* Input, FSpatialHashingOutput* Output,
		int32 ShapeIndex, const FTetherShape* Shape, FString& DebugString);

	/** Compute the spatial hash key for a given shape */
	static FIntVector ComputeSpatialHashKey(const FSpatialHashingInput* Input, const FSpatialHashingOutput* Output,
		const FTetherShape* Shape, FString& DebugString);

	static bool AreBucketsAdjacent(int32 BucketA, int32 BucketB);

	static bool IsInSameOrAdjacentBucket(const FTetherShape* ShapeA, const FTetherShape* ShapeB);

public:
	static void DrawDebugBucket(FAnimInstanceProxy* Proxy, const UWorld* World, const FTransform& Transform,
		const FIntVector& BucketIndex, const FVector& BucketSize, const FColor& Color = FColor::Green,
		bool bPersistentLines = false, float LifeTime = -1.0f, float Thickness = 1.0f);

	/**
	 * Visualizes the results of the hashing process for debugging purposes.
	 * 
	 * @param Input                Pointer to the input data used in the hashing process.
	 * @param Output               Pointer to the output data containing the results of the hashing.
	 * @param Origin			   The center of the spatial hashing grid.
	 * @param PendingDebugText	   Array of Debug Texts that to be drawn by the viewport
	 * @param LifeTime             The duration for which the debug lines should be visible.
	 * @param Proxy				   Pointer to the animation instance proxy for drawing debug information.
	 * @param World                Pointer to the world context in which the debugging visualization occurs.
	 * @param bDrawAll             Whether to draw all relevant elements or just those involved in processing.
	 * @param Color                The color used for drawing the debug visualization.
	 * @param bPersistentLines     Whether the debug lines should persist beyond a single frame.
	 * @param Thickness            The thickness of the debug lines.
	 */
	virtual void DrawDebug(const TArray<FTetherShape*>* Shapes, const FSpatialHashingInput* Input,
		const FSpatialHashingOutput* Output, const FTransform& Origin,
		TArray<FTetherDebugText>* PendingDebugText = nullptr, float LifeTime = -1.f,
		FAnimInstanceProxy* Proxy = nullptr, const UWorld* World = nullptr, bool bDrawAll = true,
		const FColor& Color = FColor::Green, bool bPersistentLines = false, float Thickness = 1.f) const override;
};
