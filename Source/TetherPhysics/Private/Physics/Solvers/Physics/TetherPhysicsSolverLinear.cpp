// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Physics/TetherPhysicsSolverLinear.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherPhysicsSolverLinear)

void UTetherPhysicsSolverLinear::Solve(const FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const
{
	const auto* Input = InputData->GetDataIO<FLinearInput>();
	auto* Output = OutputData->GetDataIO<FLinearOutput>();

	for (auto& ShapeItr : Input->ShapeSettings)
	{
		// const FTetherShape* const& Shape = ShapeItr.Key;
		const FLinearInputSettings& Settings = ShapeItr.Value;
		
		// Calculate linear acceleration based on the net force and mass
		FVector Acceleration = (Settings.Force - Settings.FrictionForce) / Settings.Mass;
	
		// Update linear velocity using the acceleration
		Output->LinearVelocity += Acceleration * DeltaTime;
	
		// Apply damping to slow down linear velocity
		switch (Settings.DampingModel)
		{
		case ETetherDampingModel::SimpleLinear:
			Output->LinearVelocity *= (1.f - Settings.LinearDamping * DeltaTime);
			break;
		case ETetherDampingModel::ExponentialDecay:
			Output->LinearVelocity *= FMath::Exp(-Settings.LinearDamping * DeltaTime);
			break;
		}
	
		// Apply linear drag due to air resistance, which is proportional to the square of the velocity
		float DragForce = Settings.LinearDragCoefficient * Output->LinearVelocity.SizeSquared();
		FVector Drag = -Output->LinearVelocity.GetSafeNormal() * DragForce * DeltaTime;
		Output->LinearVelocity += Drag;
	
		// Clamp the linear velocity to the maximum allowed value
		float LinearVelocityMagnitude = Output->LinearVelocity.Size();
		if (LinearVelocityMagnitude > Settings.MaxLinearVelocity)
		{
			Output->LinearVelocity *= Settings.MaxLinearVelocity / LinearVelocityMagnitude;
		}
	}
}
