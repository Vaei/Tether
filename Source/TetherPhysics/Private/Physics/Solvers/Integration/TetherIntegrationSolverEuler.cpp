// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Integration/TetherIntegrationSolverEuler.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherIntegrationSolverEuler)

void UTetherIntegrationSolverEuler::Solve(const FTetherIO* InputData, FTetherIO* OutputData, float DeltaTime) const
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
		const FVector& CurrentPosition = ShapeOutput.Transform.GetLocation();

		// Use the linear velocity calculated by the linear solver
		const FVector& LinearVelocity = LinearOutput.LinearVelocity;

		// Update position using Euler integration
		const FVector NewPosition = CurrentPosition + LinearVelocity * DeltaTime;

		// Update the output data for position
		ShapeOutput.Transform.SetLocation(NewPosition);

		// ---- Angular Motion (Rotation) ----

		// Get the current rotation from the transform
		const FQuat CurrentRotation = ShapeOutput.Transform.GetRotation();

		// Use the angular velocity calculated by the angular solver
		const FVector AngularVelocity = AngularOutput.AngularVelocity;

		// Update rotation using Euler integration
		const FQuat AngularDelta { AngularVelocity, AngularVelocity.Size() * DeltaTime };
		FQuat NewRotation = CurrentRotation * AngularDelta;

		// Normalize the quaternion to avoid floating-point errors
		NewRotation.Normalize();

		// Update the output data for rotation
		ShapeOutput.Transform.SetRotation(NewRotation);
	}
}
