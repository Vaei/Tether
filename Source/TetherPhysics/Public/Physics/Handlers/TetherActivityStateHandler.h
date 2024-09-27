// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherActivityStateHandler.generated.h"

struct FTetherDebugText;

/**
 * Activity State Handler for the Tether physics system.
 *
 * UTetherActivityStateHandler is responsible for managing the transition between wake and sleep states of physics objects.
 * This includes determining when an object should wake up based on forces, collisions, or movement, as well as when
 * it can safely go to sleep to reduce unnecessary computations.
 *
 * This class handles pre-solve wake checks and post-solve sleep checks and can also visualize the activity state 
 * of objects for debugging purposes.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherActivityStateHandler : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Pre-solve wake handling.
	 * 
	 * This method is called before the physics solvers are applied to determine if the object should be awake.
	 * It checks for conditions such as recent collisions, applied forces, or motion that would cause the object
	 * to wake from sleep.
	 *
	 * @param Shape				The shape being checked for wake state.
	 * @param InputData			General input data related to the object's state.
	 * @param LinearInputData	Input data specific to linear motion.
	 * @param AngularInputData	Input data specific to angular motion.
	 * @param DeltaTime			The time step for the simulation.
	 * @param WorldTime			The current world time.
	 */
	virtual void PreSolveWake(FTetherShape* Shape, const FTetherIO* InputData, const FTetherIO* LinearInputData,
		const FTetherIO* AngularInputData, float DeltaTime, double WorldTime) const;

	/**
	 * Post-solve sleep handling.
	 * 
	 * This method is called after the physics solvers have been applied to determine if the object can go to sleep.
	 * It evaluates the object's motion and forces, and if the object is at rest or near rest, it transitions the 
	 * object to a sleep state.
	 *
	 * @param Shape					The shape being checked for sleep state.
	 * @param InputData				General input data related to the object's state.
	 * @param LinearInputData		Input data specific to linear motion.
	 * @param AngularInputData		Input data specific to angular motion.
	 * @param LinearOutputData		Output data specific to linear motion after the solvers.
	 * @param AngularOutputData		Output data specific to angular motion after the solvers.
	 * @param DeltaTime				The time step for the simulation.
	 * @param WorldTime				The current world time.
	 */
	virtual void PostSolveSleep(FTetherShape* Shape, const FTetherIO* InputData, const FTetherIO* LinearInputData,
		const FTetherIO* AngularInputData, const FTetherIO* LinearOutputData, const FTetherIO* AngularOutputData,
		float DeltaTime, double WorldTime) const;

	/**
	 * Visualizes the activity state of the object for debugging purposes.
	 * 
	 * This method draws debug information about whether the object is awake or asleep, using different colors to 
	 * represent each state.
	 *
	 * @param Shape            The shape being visualized.
	 * @param InputData        Input data related to the object's state.
	 * @param PendingDebugText Array to store debug text to be displayed.
	 * @param LifeTime         The duration for which the debug lines should be visible (negative values mean infinite).
	 * @param Proxy            Pointer to the animation instance proxy for drawing debug information.
	 * @param World            Pointer to the world context in which the debugging visualization occurs.
	 * @param WakeColor        Color to represent the wake state.
	 * @param SleepColor       Color to represent the sleep state.
	 * @param bPersistentLines Whether the debug lines should persist beyond a single frame.
	 * @param Thickness        The thickness of the debug lines.
	 */
	virtual void DrawDebug(FTetherShape* Shape, const FTetherIO* InputData,
		TArray<FTetherDebugText>* PendingDebugText = nullptr, float LifeTime = -1.f,
		FAnimInstanceProxy* Proxy = nullptr, const UWorld* World = nullptr, const FColor& WakeColor = FColor::White,
		const FColor& SleepColor = FColor::Black, bool bPersistentLines = false, float Thickness = 1.0f) const;
};
