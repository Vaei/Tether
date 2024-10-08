﻿// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherHashing.generated.h"

struct FTetherDebugText;

/**
 * Abstract base class for generic hashing systems used in physics simulations.
 *
 * UTetherHashing serves as a framework for implementing various types of hashing algorithms
 * to optimize and organize data within a physics simulation. This could include spatial hashing,
 * temporal hashing, or other forms of grouping and prioritization that are essential for efficient
 * collision detection, constraint solving, or other physics-related tasks.
 * 
 * The class is designed to be extended with specific hashing implementations that define
 * how objects or data are grouped, paired, or prioritized for processing. It also includes
 * methods for debugging and visualizing the results of the hashing process.
 * 
 * Key Responsibilities:
 * - Solve: Implement the core logic for pairing or grouping objects/data based on a specific hashing algorithm.
 * - DrawDebug: Visualize the results of the hashing process for debugging and optimization.
 * 
 * Note: This class is abstract and cannot be instantiated directly. It is intended to be subclassed
 * to implement specific hashing algorithms. It is also not intended for use within Blueprints.
 */
UCLASS(Abstract, NotBlueprintable)
class TETHERPHYSICS_API UTetherHashing : public UObject
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
		const FTransform& Origin, float DeltaTime, double WorldTime) const {}

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
		const FColor& Color = FColor::Green, bool bPersistentLines = false, float Thickness = 1.f) const {}
};
