// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherReplay.generated.h"

/**
 * Handles the recording and playback of physics simulation states.
 *
 * UTetherReplay is responsible for capturing and replaying the physics states of objects during a simulation.
 * This class allows for precise tracking and recreation of object states, enabling detailed analysis, debugging,
 * or deterministic replays of recorded simulations. It is designed to interface with FTetherShape objects
 * and manage their linear and angular physics data across simulation frames.
 * 
 * Key Features:
 * - Record: Captures the physics state at a specific time, storing it for future playback.
 * - Replay: Retrieves and applies the stored physics state to accurately recreate the simulation.
 * 
 * Note: This class is not intended for use within Blueprints and is optimized for use within
 * C++ code, particularly in scenarios requiring detailed physics simulations and replay functionalities.
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherReplay : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Records the physics state of a specific object at a given time stamp.
	 * 
	 * @param RecordedData    Pointer to the structure where the recorded data will be stored.
	 * @param TimeStamp       The time at which the state is recorded.
	 * @param LinearInputData Pointer to the linear physics data to be recorded.
	 * @param AngularInputData Pointer to the angular physics data to be recorded.
	 */
	virtual void RecordPhysicsState(FTetherIO* RecordedData, double TimeStamp, const FTetherIO* LinearInputData, const FTetherIO* AngularInputData) const;

	/**
	 * Replays the physics state of a specific object at a given time stamp.
	 * 
	 * @param RecordedData    Pointer to the structure containing the recorded data.
	 * @param TimeStamp       The time at which the state should be replayed.
	 * @param TetherShapes     Pointers to the FTetherShape objects whose states are being restored.
	 * @param OutLinearInput  Pointer to the structure where the linear physics data will be output.
	 * @param OutAngularInput Pointer to the structure where the angular physics data will be output.
	 * @param ReplayMode Determines the behavior of the replay system when handling multiple shapes.
	 * @return True if the state was successfully replayed, otherwise false.
	 */
	virtual bool ReplayPhysicsState(const FTetherIO* RecordedData, double TimeStamp, FTetherIO* OutLinearInput, FTetherIO* OutAngularInput, ETetherReplayMode ReplayMode = ETetherReplayMode::Completion) const;
};
