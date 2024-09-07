// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Physics/TetherPhysicsSolverLinear.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherPhysicsSolverLinear)

void UTetherPhysicsSolverLinear::Solve(const FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform, float DeltaTime) const
{
	const auto* Input = InputData->GetDataIO<FLinearInput>();
	auto* Output = OutputData->GetDataIO<FLinearOutput>();

	for (auto& ShapeItr : Input->ShapeSettings)
	{
		const FTetherShape* const& Shape = ShapeItr.Key;
		const FLinearInputSettings& Settings = ShapeItr.Value;
		
		FLinearOutputData& LinearData = Output->ShapeData.FindOrAdd(Shape);
		FVector& LinearVelocity = LinearData.LinearVelocity;
		
		// Calculate linear acceleration based on the net force and mass
		const float Mass = FMath::Max(KINDA_SMALL_NUMBER, Settings.Mass);
		FVector Acceleration = (Settings.Force - Settings.FrictionForce) / Mass;
		Acceleration += Settings.Acceleration;
	
		// Update linear velocity using the acceleration
		LinearVelocity += Acceleration * DeltaTime;
	
		// Apply damping to slow down linear velocity
		switch (Settings.DampingModel)
		{
		case ETetherDampingModel::SimpleLinear:
			LinearVelocity *= (1.f - Settings.LinearDamping * DeltaTime);
			break;
		case ETetherDampingModel::ExponentialDecay:
			LinearVelocity *= FMath::Exp(-Settings.LinearDamping * DeltaTime);
			break;
		}
	
		// Apply linear drag due to air resistance, which is proportional to the square of the velocity
		float DragForce = Settings.LinearDragCoefficient * LinearVelocity.SizeSquared();
		FVector Drag = -LinearVelocity.GetSafeNormal() * DragForce * DeltaTime;
		LinearVelocity += Drag;
	
		// Clamp the linear velocity to the maximum allowed value
		float LinearVelocityMagnitude = LinearVelocity.Size();
		if (LinearVelocityMagnitude > Settings.MaxLinearVelocity)
		{
			if (!FMath::IsNearlyZero(LinearVelocityMagnitude))
			{
				LinearVelocity *= Settings.MaxLinearVelocity / LinearVelocityMagnitude;
			}
			else
			{
				LinearVelocity = FVector::ZeroVector;
			}
		}

		// UE_LOG(LogTemp, Log, TEXT("LinearVelocity %s"), *LinearVelocity.ToString());
	}
}
