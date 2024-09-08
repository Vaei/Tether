// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Integration/TetherIntegrationSolverRK4.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherIntegrationSolverRK4)

void UTetherIntegrationSolverRK4::Solve(const FTetherIO* InputData, FTetherIO* OutputData, float DeltaTime) const
{
	const auto* Input = InputData->GetDataIO<FIntegrationInput>();
	auto* Output = OutputData->GetDataIO<FIntegrationOutput>();

	Output->Shapes.Reset();

	// Loop through each shape being simulated
	for (const auto& ShapeItr : *Input->Shapes)
	{
		// Retrieve shape data
		const FTetherShape* const& Shape = ShapeItr.Key;
		const FTransform* CurrentTransform = ShapeItr.Value;
		FTransform Transform = *CurrentTransform;
		
		// Get the shape-specific input and output data
		const FLinearOutputData& LinearOutput = Input->LinearOutput->ShapeData[Shape];
		const FAngularOutputData& AngularOutput = Input->AngularOutput->ShapeData[Shape];

		// ---- Linear Motion (Position) ----

		// Get the current position from the transform
		const FVector& CurrentPosition = Transform.GetLocation();

		// Use the linear velocity calculated by the linear solver
		const FVector& LinearVelocity = LinearOutput.LinearVelocity;

		// Calculate intermediate steps for RK4 integration
		const FVector K1 = LinearVelocity * DeltaTime;					// Slope at beginning of interval
		const FVector K2 = (LinearVelocity + 0.5f * K1) * DeltaTime;	// Slope at mid-point of interval (K1)
		const FVector K3 = (LinearVelocity + 0.5f * K2) * DeltaTime;	// Slope at mid-point of interval (K2)
		const FVector K4 = (LinearVelocity + K3) * DeltaTime;			// Slope at end of interval

		// Update position using RK4 integration - Weighted sum of slopes
		const FVector NewPosition = CurrentPosition + (K1 + 2.f * K2 + 2.f * K3 + K4) / 6.f;

		// Update the output data for position
		Transform.SetLocation(NewPosition);

		// ---- Angular Motion (Rotation) ----

		// Get the current rotation from the transform
		const FQuat CurrentRotation = Transform.GetRotation();

		// Use the angular velocity calculated by the angular solver
		const FVector AngularVelocity = AngularOutput.AngularVelocity;

		// Calculate angular acceleration and corresponding angular velocities for RK4 steps
		const FQuat K1Rot = FQuat(AngularVelocity, AngularVelocity.Size() * DeltaTime);
		const FQuat K2Rot = FQuat(AngularVelocity + 0.5f * AngularVelocity * DeltaTime, (AngularVelocity.Size() + 0.5f * K1Rot.GetAngle()) * DeltaTime);
		const FQuat K3Rot = FQuat(AngularVelocity + 0.5f * AngularVelocity * DeltaTime, (AngularVelocity.Size() + 0.5f * K2Rot.GetAngle()) * DeltaTime);
		const FQuat K4Rot = FQuat(AngularVelocity + AngularVelocity * DeltaTime, (AngularVelocity.Size() + K3Rot.GetAngle()) * DeltaTime);

		// Apply RK4 integration for rotation, using quaternion multiplication
		FQuat NewRotation = CurrentRotation * (K1Rot * 2.f * K2Rot * 2.f * K3Rot * K4Rot);

		// Normalize the quaternion to avoid floating-point errors
		NewRotation.Normalize();

		// Update the output data for rotation
		Transform.SetRotation(NewRotation);
		Output->Shapes.Add(Shape, Transform);
	}
}
