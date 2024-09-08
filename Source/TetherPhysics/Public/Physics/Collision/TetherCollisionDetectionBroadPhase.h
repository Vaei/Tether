// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "Shapes/TetherShape.h"
#include "UObject/Object.h"
#include "TetherCollisionDetectionBroadPhase.generated.h"

class UTetherCollisionDetectionHandler;
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
	 * This typically involves converting to bounding boxes to compare overlaps.
	 *
	 * @param InputData						The input data structure containing relevant information, such as the array of shapes representing objects in the simulation and other spatial hashing data.
	 * @param OutputData					The output data structure that will store the pairs of objects that should be further tested in the narrow-phase collision detection.
	 * @param CollisionDetectionHandler		The handler to use for collision detection
	 */
	virtual void DetectCollision(const FTetherIO* InputData, FTetherIO* OutputData, const UTetherCollisionDetectionHandler* CollisionDetectionHandler) const;

	/**
	 * Visualize the results of the broad-phase collision detection.
	 *
	 * @param InputData				The input data structure containing relevant information used during the broad-phase collision detection, such as object shapes.
	 * @param OutputData			The output data structure containing the results of the broad-phase collision detection, including potential collision pairs.
	 * @param PendingDebugText		Array of Debug Texts that to be drawn by the viewport
	 * @param LifeTime				How long to draw the debug shapes for
	 * @param Proxy					Pointer to the animation instance proxy for drawing debug information (if applicable).
	 * @param World					The world context in which the debug information will be drawn (optional).
	 * @param bForceDraw			Whether to force the drawing of debug lines regardless of the usual conditions (optional).
	 * @param NoTestColor			The default color used for drawing debug information for shapes that incurred no overlap testing (optional).
	 * @param NoOverlapColor		The color used for drawing debug information where collisions have NOT been detected (optional).
	 * @param OverlapColor			The color used for drawing debug information where collisions have been detected (optional).
	 */
	virtual void DrawDebug(const FTetherIO* InputData, const FTetherIO* OutputData,
		TArray<FTetherDebugText>* PendingDebugText = nullptr, float LifeTime = -1.f,
		FAnimInstanceProxy* Proxy = nullptr, UWorld* World = nullptr, bool bForceDraw = false,
		const FColor& NoTestColor = FColor::White, const FColor& OverlapColor = FColor::Orange,
		const FColor& NoOverlapColor = FColor::Yellow) const;
};
