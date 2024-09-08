// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shapes/TetherShape.h"
#include "UObject/Object.h"
#include "TetherCollisionDetectionNarrowPhase.generated.h"

struct FTetherNarrowPhaseCollisionOutput;

/**
 * Base class for narrow-phase collision detection in the Tether physics system.
 *
 * UTetherCollisionDetectionNarrowPhase is responsible for performing detailed collision checks between
 * pairs of objects that have been identified as potential collisions during the broad-phase. This phase
 * involves precise geometry-based checks to determine if and where collisions occur, calculating
 * contact points, penetration depths, and other necessary information for resolving collisions.
 *
 * This class is designed to be extended with specific narrow-phase algorithms, such as SAT (Separating
 * Axis Theorem), GJK (Gilbert-Johnson-Keerthi), or other advanced collision detection methods.
 * It also provides methods for debugging the narrow-phase collision detection process.
 *
 * Key Responsibilities:
 * - DetectCollision: Implement the logic for accurately detecting collisions and computing collision details.
 * - DrawDebug: Visualize the results of the narrow-phase collision detection for debugging and analysis.
 *
 * Note: This class is not intended for use within Blueprints and should be extended in C++ to implement
 * custom narrow-phase collision detection algorithms.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherCollisionDetectionNarrowPhase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Perform detailed collision detection between objects using narrow-phase techniques.
	 *
	 * @param BoneShapes The array of shapes representing objects in the simulation.
	 * @param Output The output array that will store the detailed collision information for each detected collision.
	 */
	virtual void DetectCollision(const TArray<FTetherShape>& BoneShapes, TArray<FTetherNarrowPhaseCollisionOutput>& Output) const;

	/**
	 * Visualize the results of the narrow-phase collision detection.
	 *
	 * @param CollisionOutput The results of the narrow-phase collision detection, including contact points and penetration depths.
	 * @param Proxy Pointer to the animation instance proxy for drawing debug information.
	 */
	virtual void DrawDebug(const TArray<FTetherNarrowPhaseCollisionOutput>& CollisionOutput, FAnimInstanceProxy* Proxy) const;
};
