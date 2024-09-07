// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Physics/TetherPhysicsSolverAngular.h"

#include "TetherStatics.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherPhysicsSolverAngular)

void UTetherPhysicsSolverAngular::Solve(const FTetherIO* InputData, FTetherIO* OutputData, const FTransform& Transform,
	float DeltaTime) const
{
	const auto* Input = InputData->GetDataIO<FAngularInput>();
	auto* Output = OutputData->GetDataIO<FAngularOutput>();

	for (auto& ShapeItr : Input->ShapeSettings)
	{
		const FTetherShape* const& Shape = ShapeItr.Key;
		const FAngularInputSettings& Settings = ShapeItr.Value;

		FAngularOutputData& AngularData = Output->ShapeData.FindOrAdd(Shape);
		FVector& AngularVelocity = AngularData.AngularVelocity;
		FVector& Inertia = AngularData.Inertia;

		const FTetherShape_AxisAlignedBoundingBox AABB = Shape->GetTetherShapeObject()->GetBoundingBox(*Shape);
		const FVector BoxExtent = AABB.GetBoxExtents();
	
		// Use dynamic inertia calculation based on BoxExtent if bUseDynamicInertia is true
		if (Settings.bUseDynamicInertia)
		{
			Inertia = FVector
			{
				(Settings.Mass * (BoxExtent.Y * BoxExtent.Y + BoxExtent.Z * BoxExtent.Z)) / FTether::MomentOfInertia,
				(Settings.Mass * (BoxExtent.X * BoxExtent.X + BoxExtent.Z * BoxExtent.Z)) / FTether::MomentOfInertia,
				(Settings.Mass * (BoxExtent.X * BoxExtent.X + BoxExtent.Y * BoxExtent.Y)) / FTether::MomentOfInertia
			};
		}
		else
		{
			Inertia = Settings.Inertia;  // Use the predefined inertia value
		}

		FVector Torque = FVector::CrossProduct(Settings.PointOfApplication - Settings.CenterOfMass, Settings.Torque);

		// Calculate angular acceleration based on the net torque and inertia
		FVector AngularAcceleration = (Torque - Settings.FrictionTorque) * (FVector::OneVector / Inertia);

		// Update angular velocity using the angular acceleration
		AngularVelocity += AngularAcceleration * DeltaTime;

		// Apply damping to slow down angular velocity
		switch (Settings.DampingModel)
		{
		case ETetherDampingModel::SimpleLinear:
			AngularVelocity *= (1.f - Settings.AngularDamping * DeltaTime);
			break;
		case ETetherDampingModel::ExponentialDecay:
			AngularVelocity *= FMath::Exp(-Settings.AngularDamping * DeltaTime);
			break;
		}

		// Apply angular drag due to air resistance, which is proportional to the square of the velocity
		float DragForce = Settings.AngularDragCoefficient * AngularVelocity.SizeSquared();
		FVector DragTorque = -AngularVelocity.GetSafeNormal() * DragForce * DeltaTime;
		AngularVelocity += DragTorque;

		// Clamp the angular velocity to the maximum allowed value
		float AngularVelocityMagnitude = AngularVelocity.Size();
		if (AngularVelocityMagnitude > Settings.MaxAngularVelocity)
		{
			AngularVelocity *= Settings.MaxAngularVelocity / AngularVelocityMagnitude;
		}
	}
}
