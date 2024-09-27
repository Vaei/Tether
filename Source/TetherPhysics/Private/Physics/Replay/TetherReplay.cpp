// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Replay/TetherReplay.h"

#include "TetherStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherReplay)

void UTetherReplay::RecordPhysicsState(const FTetherShape* Shape, FTetherIO* RecordedData, double TimeStamp,
	const FTetherIO* LinearInputData, const FTetherIO* AngularInputData) const
{
	auto* Record = RecordedData->GetDataIO<FRecordedPhysicsData>();
	const auto* LinearInput = LinearInputData->GetDataIO<FLinearInput>();
	const auto* AngularInput = AngularInputData->GetDataIO<FAngularInput>();

	FRecordedPhysicsObject* ObjectRecording = Record->FindOrCreateObjectRecording(Shape);
	if (ObjectRecording)
	{
		ObjectRecording->AddFrame(TimeStamp, LinearInput, AngularInput);

		// Log the physics state to Visual Logger
		UE_VLOG(this, LogTetherReplay, Log, TEXT("Recorded Physics State for Shape: %s at Time: %f"), *Shape->GetName(), TimeStamp);
	}
}

bool UTetherReplay::ReplayPhysicsState(const FTetherShape* Shape, const FTetherIO* RecordedData, double TimeStamp, FTetherIO* OutLinearInput, FTetherIO*
	OutAngularInput, ETetherReplayMode ReplayMode) const
{
	auto* Record = RecordedData->GetDataIO<FRecordedPhysicsData>();
	bool bSuccess = false;

	// Iterate over each TetherShape in the provided array
	const FRecordedPhysicsObject* ObjectRecording = Record->RecordedObjects.FindByPredicate([&](const FRecordedPhysicsObject& Obj)
	{
		return Obj.TetherShape == Shape;
	});

	if (ObjectRecording)
	{
		// Check each frame within the object recording for a matching timestamp
		for (const FRecordedPhysicsFrame& Frame : ObjectRecording->RecordedFrames)
		{
			if (FMath::IsNearlyEqual(Frame.TimeStamp, TimeStamp, KINDA_SMALL_NUMBER))
			{
				// Set the output data for the matching frame
				OutLinearInput->SetDataIO<FLinearInput>(Frame.LinearInput);
				OutAngularInput->SetDataIO<FAngularInput>(Frame.AngularInput);
				
				bSuccess = true;
				
				// Log the replayed physics state
				UE_VLOG(this, LogTetherReplay, Log, TEXT("Replayed Physics State for Shape: %s at Time: %f"), *Shape->GetName(), TimeStamp);

				// In ShortCircuit mode, return immediately after the first successful match
				if (ReplayMode == ETetherReplayMode::ShortCircuit)
				{
					return true;
				}

				// Continue checking other shapes in Completion mode
			}
		}
	}
	
	// Return true if at least one TetherShape successfully replayed its state, otherwise false
	return bSuccess;
}
