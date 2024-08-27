// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shapes/TetherShape.h"
#include "UObject/Object.h"
#include "TetherCollisionDetectionBroadPhase.generated.h"

struct FTetherBroadPhaseCollisionOutput;

/**
 * Base class for broad-phase collision detection in the Tether physics system.
 *
 * UTetherCollisionDetectionBroadPhase is responsible for quickly identifying potential collisions
 * between objects in the physics simulation. The broad-phase collision detection reduces the number
 * of collision checks by filtering out pairs of objects that are unlikely to collide, passing only the
 * relevant pairs to the narrow-phase collision detection for detailed analysis.

 * Key Responsibilities:
 * - DetectCollision: Implement the logic for identifying potential collision pairs.
 * - DrawDebug: Visualize the results of the broad-phase collision detection for debugging and optimization.
 *
 * Note: This class is not intended for use within Blueprints and should be extended in C++ to implement
 * custom broad-phase collision detection algorithms.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherCollisionDetectionBroadPhase : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Detect potential collisions between objects using broad-phase techniques.
	 *
	 * @param BoneShapes The array of shapes representing objects in the simulation.
	 * @param Output The output data structure that will store the pairs of objects that should be further tested in the narrow-phase.
	 */
	virtual void DetectCollision(const TArray<FTetherShape>& BoneShapes, FTetherBroadPhaseCollisionOutput& Output) const;

	/**
	 * Visualize the results of the broad-phase collision detection.
	 *
	 * @param BoneShapes The array of shapes representing objects in the simulation.
	 * @param CollisionOutput The results of the broad-phase collision detection.
	 * @param AnimInstanceProxy Pointer to the animation instance proxy for drawing debug information.
	 * @param bForceDraw Whether to force drawing of debug lines even if certain conditions are not met.
	 */
	virtual void DrawDebug(const TArray<FTetherShape>& BoneShapes, const FTetherBroadPhaseCollisionOutput& CollisionOutput, FAnimInstanceProxy* AnimInstanceProxy, bool bForceDraw = false) const;
};
