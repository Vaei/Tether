// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Physics/TetherPhysicsSolverAngular.h"

#include "TetherStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherPhysicsSolverAngular)

void UTetherPhysicsSolverAngular::Solve(const FAngularInput& Input, FAngularOutput& Output, const FTransform& Transform, float DeltaTime,
	float Radius) const
{
	// @todo Is it Radius / 2.f?
	Solve(Input, Output, Transform, DeltaTime, FVector(Radius));
}

void UTetherPhysicsSolverAngular::Solve(const FAngularInput& Input, FAngularOutput& Output, const FTransform& Transform, float DeltaTime,
	float HalfHeight, float Radius) const
{
	// @todo Is it HalfHeight & Radius / 2.f?
	Solve(Input, Output, Transform, DeltaTime, FVector(Radius, Radius, HalfHeight));
}

void UTetherPhysicsSolverAngular::Solve(const FAngularInput& Input, FAngularOutput& Output, const FTransform& Transform, float DeltaTime,
	const FVector& BoxExtent) const
{
	// Use dynamic inertia calculation based on BoxExtent if bUseDynamicInertia is true
	if (Input.bUseDynamicInertia)
	{
		Output.Inertia = FVector
		{
			(Input.Mass * (BoxExtent.Y * BoxExtent.Y + BoxExtent.Z * BoxExtent.Z)) / FTether::MomentOfInertia,
			(Input.Mass * (BoxExtent.X * BoxExtent.X + BoxExtent.Z * BoxExtent.Z)) / FTether::MomentOfInertia,
			(Input.Mass * (BoxExtent.X * BoxExtent.X + BoxExtent.Y * BoxExtent.Y)) / FTether::MomentOfInertia
		};
	}
	else
	{
		Output.Inertia = Input.Inertia;  // Use the predefined inertia value
	}

	FVector Torque = FVector::CrossProduct(Input.PointOfApplication - Input.CenterOfMass, Input.Torque);

	// Calculate angular acceleration based on the net torque and inertia
	FVector AngularAcceleration = (Torque - Input.FrictionTorque) * (FVector::OneVector / Output.Inertia);

	// Update angular velocity using the angular acceleration
	Output.AngularVelocity += AngularAcceleration * DeltaTime;

	// Apply damping to slow down angular velocity
	switch (Input.DampingModel)
	{
	case ETetherDampingModel::SimpleLinear:
		Output.AngularVelocity *= (1.f - Input.AngularDamping * DeltaTime);
		break;
	case ETetherDampingModel::ExponentialDecay:
		Output.AngularVelocity *= FMath::Exp(-Input.AngularDamping * DeltaTime);
		break;
	}

	// Apply angular drag due to air resistance, which is proportional to the square of the velocity
	float DragForce = Input.AngularDragCoefficient * Output.AngularVelocity.SizeSquared();
	FVector DragTorque = -Output.AngularVelocity.GetSafeNormal() * DragForce * DeltaTime;
	Output.AngularVelocity += DragTorque;

	// Clamp the angular velocity to the maximum allowed value
	float AngularVelocityMagnitude = Output.AngularVelocity.Size();
	if (AngularVelocityMagnitude > Input.MaxAngularVelocity)
	{
		Output.AngularVelocity *= Input.MaxAngularVelocity / AngularVelocityMagnitude;
	}
}
