// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Integration/TetherIntegrationSolverEuler.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherIntegrationSolverEuler)

void UTetherIntegrationSolverEuler::Solve(const FTetherIO* InputData, FTetherIO* OutputData, float DeltaTime) const
{
	const auto* Input = InputData->GetDataIO<FIntegrationInput>();
	auto* Output = OutputData->GetDataIO<FIntegrationOutput>();

	// Loop through each shape being simulated
	for (const auto& ShapeItr : *Input->Shapes)
	{
		// Retrieve shape data
		const FTetherShape* const& Shape = ShapeItr.Key;

		// Grab the transform from output if it exists due to updating multiple times per tick; otherwise from Input on first sub-tick
		const FTransform* CurrentTransform = Output->Shapes.Contains(Shape) ? &Output->Shapes[Shape] : ShapeItr.Value;
		FTransform Transform = *CurrentTransform;
		
		// Get the shape-specific input and output data
		const FLinearOutputData& LinearOutput = Input->LinearOutput->ShapeData[Shape];
		const FAngularOutputData& AngularOutput = Input->AngularOutput->ShapeData[Shape];

		// ---- Linear Motion (Position) ----

		// Get the current position from the transform
		const FVector& CurrentPosition = Transform.GetLocation();

		// Use the linear velocity calculated by the linear solver
		const FVector& LinearVelocity = LinearOutput.LinearVelocity;

		// Update position using Euler integration
		const FVector NewPosition = CurrentPosition + LinearVelocity * DeltaTime;

		// Update the output data for position
		Transform.SetLocation(NewPosition);

		// ---- Angular Motion (Rotation) ----

		// Get the current rotation from the transform
		const FQuat CurrentRotation = Transform.GetRotation();

		// Use the angular velocity calculated by the angular solver
		const FVector AngularVelocity = AngularOutput.AngularVelocity;

		// Update rotation using Euler integration
		const FQuat AngularDelta { AngularVelocity, AngularVelocity.Size() * DeltaTime };
		FQuat NewRotation = CurrentRotation * AngularDelta;

		// Normalize the quaternion to avoid floating-point errors
		NewRotation.Normalize();

		// Update the output data for rotation
		Transform.SetRotation(NewRotation);

		// Update or Add Transform
		if (Output->Shapes.Contains(Shape))
		{
			Output->Shapes[Shape] = Transform;
		}
		else
		{
			Output->Shapes.Add(Shape, Transform);
		}
	}
}
