// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Replay/TetherReplay.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherReplay)

void UTetherReplay::RecordPhysicsState(FTetherIO* RecordedData, float TimeStamp, FTetherShape* TetherShape,
	const FTetherIO* LinearInputData, const FTetherIO* AngularInputData) const
{
	auto* Record = RecordedData->GetDataIO<FRecordedPhysicsData>();
	const auto* LinearInput = LinearInputData->GetDataIO<FLinearInput>();
	const auto* AngularInput = AngularInputData->GetDataIO<FAngularInput>();

	FRecordedPhysicsObject* ObjectRecording = Record->FindOrCreateObjectRecording(TetherShape);
	if (ObjectRecording)
	{
		ObjectRecording->AddFrame(TimeStamp, LinearInput, AngularInput);
	}
}

bool UTetherReplay::ReplayPhysicsState(const FTetherIO* RecordedData, float TimeStamp, FTetherShape* TetherShape,
	FTetherIO* OutLinearInput, FTetherIO* OutAngularInput) const
{
	auto* Record = RecordedData->GetDataIO<FRecordedPhysicsData>();
	const FRecordedPhysicsObject* ObjectRecording = Record->RecordedObjects.FindByPredicate([&](const FRecordedPhysicsObject& Obj)
	{
		return Obj.TetherShape == TetherShape;
	});

	if (ObjectRecording)
	{
		for (const FRecordedPhysicsFrame& Frame : ObjectRecording->RecordedFrames)
		{
			if (FMath::IsNearlyEqual(Frame.TimeStamp, TimeStamp, KINDA_SMALL_NUMBER))
			{
				OutLinearInput->SetDataIO<FLinearInput>(Frame.LinearInput);
				OutAngularInput->SetDataIO<FAngularInput>(Frame.AngularInput);
				return true;
			}
		}
	}

	return false;
}
