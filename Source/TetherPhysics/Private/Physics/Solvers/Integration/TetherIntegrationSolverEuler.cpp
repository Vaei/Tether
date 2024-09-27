// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Integration/TetherIntegrationSolverEuler.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherIntegrationSolverEuler)

void UTetherIntegrationSolverEuler::Solve(const FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData, float DeltaTime, double WorldTime) const
{
	const auto* Input = InputData->GetDataIO<FIntegrationInput>();
	auto* Output = OutputData->GetDataIO<FIntegrationOutput>();

	// Copy the transform
	FTransform Transform = Shape->GetAppliedWorldTransform();
	
	// Get the shape-specific input and output data
	const FLinearOutput* const& LinearOutput = Input->LinearOutput;
	const FAngularOutput* const& AngularOutput = Input->AngularOutput;

	// ---- Linear Motion (Position) ----

	// Get the current position from the transform
	const FVector& CurrentPosition = Transform.GetLocation();

	// Use the linear velocity calculated by the linear solver
	const FVector& LinearVelocity = LinearOutput->LinearVelocity;

	// Update position using Euler integration
	const FVector NewPosition = CurrentPosition + LinearVelocity * DeltaTime;

	// Update the output data for position
	Transform.SetLocation(NewPosition);

	// ---- Angular Motion (Rotation) ----

	// Get the current rotation from the transform
	const FQuat CurrentRotation = Transform.GetRotation();

	// Use the angular velocity calculated by the angular solver
	const FVector AngularVelocity = AngularOutput->AngularVelocity;

	// Update rotation using Euler integration
	const FQuat AngularDelta { AngularVelocity, AngularVelocity.Size() * DeltaTime };
	FQuat NewRotation = CurrentRotation * AngularDelta;

	// Normalize the quaternion to avoid floating-point errors
	NewRotation.Normalize();

	// Update the output data for rotation
	Transform.SetRotation(NewRotation);

	// Update Transform
	Output->Transform = Transform;
}