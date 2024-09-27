// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherCollisionDetectionNarrowPhase.generated.h"

struct FTetherDebugText;
class UTetherCollisionDetectionHandler;
struct FNarrowPhaseOutput;

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
	 * Perform narrow-phase collision detection between pairs of objects identified in the broad phase.
	 * This involves detailed checks between the shapes of the objects to detect actual collisions,
	 * calculate contact points, penetration depth, and other collision response data.
	 *
	 * @param InputData						The input data structure containing the potential collision pairs from the broad-phase, as well as shape and transformation data.
	 * @param OutputData					The output data structure that will store detailed collision information, such as contact points, penetration depth, and collision normal.
	 * @param CollisionDetectionHandler		The handler responsible for performing the narrow-phase collision checks for specific shape types.
	 * @param WorldTime						Current WorldTime appended by TimeTicks
	 */
	virtual void DetectCollision(const FTetherIO* InputData, FTetherIO* OutputData, const UTetherCollisionDetectionHandler* CollisionDetectionHandler, float DeltaTime, double WorldTime) const;
	
	virtual void DrawDebug(const TArray<FTetherShape*>* Shapes, const FTetherIO* InputData, const FTetherIO* OutputData, TArray<FTetherDebugText>* PendingDebugText = nullptr,
		float LifeTime = -1.f, FAnimInstanceProxy* Proxy = nullptr, const UWorld* World = nullptr, const FColor& CollisionColor = FColor::Red, const FColor& NoCollisionColor = FColor::Blue, const FColor& InfoColor = FColor::Orange, const FColor& TextColor = FColor::White, bool bPersistentLines = false, float Thickness = 0.f) const;
};
