// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Physics/TetherPhysicsSolverLinear.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherPhysicsSolverLinear)

void UTetherPhysicsSolverLinear::Solve(const FLinearInput& Input, FLinearOutput& Output, const FTransform& Transform, float DeltaTime) const
{
	// Calculate linear acceleration based on the net force and mass
	FVector Acceleration = (Input.Force - Input.FrictionForce) / Input.Mass;

	// Update linear velocity using the acceleration
	Output.LinearVelocity += Acceleration * DeltaTime;

	// Apply damping to slow down linear velocity
	switch (Input.DampingModel)
	{
	case ETetherDampingModel::SimpleLinear:
		Output.LinearVelocity *= (1.f - Input.LinearDamping * DeltaTime);
		break;
	case ETetherDampingModel::ExponentialDecay:
		Output.LinearVelocity *= FMath::Exp(-Input.LinearDamping * DeltaTime);
		break;
	}

	// Apply linear drag due to air resistance, which is proportional to the square of the velocity
	float DragForce = Input.LinearDragCoefficient * Output.LinearVelocity.SizeSquared();
	FVector Drag = -Output.LinearVelocity.GetSafeNormal() * DragForce * DeltaTime;
	Output.LinearVelocity += Drag;

	// Clamp the linear velocity to the maximum allowed value
	float LinearVelocityMagnitude = Output.LinearVelocity.Size();
	if (LinearVelocityMagnitude > Input.MaxLinearVelocity)
	{
		Output.LinearVelocity *= Input.MaxLinearVelocity / LinearVelocityMagnitude;
	}
}
