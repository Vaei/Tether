// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Integration/TetherIntegrationSolverVerlet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherIntegrationSolverVerlet)

void UTetherIntegrationSolverVerlet::Solve(const FTetherIO* InputData, FTetherIO* OutputData, float DeltaTime) const
{
	const auto* Input = InputData->GetDataIO<FIntegrationInput>();
	auto* Output = OutputData->GetDataIO<FIntegrationOutput>();

	// Loop through each shape being simulated
	for (const FTetherShape* Shape : Input->Shapes)
	{
		// Get the shape-specific input and output data
		const FLinearOutputData& LinearOutput = Input->LinearOutput.ShapeData[Shape];
		const FAngularOutputData& AngularOutput = Input->AngularOutput.ShapeData[Shape];
		FIntegrationOutputData& ShapeOutput = Output->ShapeTransform[Shape];

		// ---- Linear Motion (Position) ----

		// Get the current position from the transform
		FVector CurrentPosition = ShapeOutput.Transform.GetLocation();

		// Use the linear velocity calculated by the linear solver
		FVector LinearVelocity = LinearOutput.LinearVelocity;  // This velocity comes from the linear solver

		// Update position using Verlet integration with the calculated linear velocity
		FVector NewPosition = CurrentPosition + LinearVelocity * DeltaTime;

		// Update the output data for position and velocity
		ShapeOutput.Transform.SetLocation(NewPosition);  // Store the new calculated position

		// ---- Angular Motion (Rotation) ----

		// Get the current rotation from the transform
		FQuat CurrentRotation = ShapeOutput.Transform.GetRotation();

		// Use the angular velocity calculated by the angular solver
		FVector AngularVelocity = AngularOutput.AngularVelocity;  // This velocity comes from the angular solver

		// Update rotation using Verlet integration with the calculated angular velocity
		FQuat AngularDelta = FQuat(AngularVelocity, AngularVelocity.Size() * DeltaTime);
		FQuat NewRotation = CurrentRotation * AngularDelta;

		// Normalize the quaternion to avoid floating point errors
		NewRotation.Normalize();

		// Update the output data for rotation and angular velocity
		ShapeOutput.Transform.SetRotation(NewRotation);  // Store the new calculated rotation
	}
}
