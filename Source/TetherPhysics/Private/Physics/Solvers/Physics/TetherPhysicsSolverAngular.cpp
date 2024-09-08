﻿// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Physics/TetherPhysicsSolverAngular.h"

#include "TetherStatics.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherPhysicsSolverAngular)

namespace FTether
{
#if ENABLE_DRAW_DEBUG
	TAutoConsoleVariable<bool> CVarTetherSolverAngularDraw(TEXT("p.Tether.Solver.Angular.Draw"), false, TEXT("Draw Tether Angular Solver Helpers"));
#endif
}

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

		FVector Torque;
		if (Settings.PointOfApplication.IsNearlyZero())
		{
			// Apply the torque directly if there's no lever arm
			Torque = Settings.Torque;
		}
		else
		{
			// Calculate the torque based on the lever arm (cross product)
			Torque = FVector::CrossProduct(Settings.PointOfApplication - Settings.CenterOfMass, Settings.Torque);
		}

		// Clamp small inertia values to avoid division by zero
		Inertia.X = FMath::Max(Inertia.X, KINDA_SMALL_NUMBER);
		Inertia.Y = FMath::Max(Inertia.Y, KINDA_SMALL_NUMBER);
		Inertia.Z = FMath::Max(Inertia.Z, KINDA_SMALL_NUMBER);

		// Calculate angular acceleration based on the net torque and inertia
		const FVector AngularAcceleration = (Torque - Settings.FrictionTorque) * (FVector::OneVector / Inertia);

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
			if (!FMath::IsNearlyZero(AngularVelocityMagnitude))
			{
				AngularVelocity *= Settings.MaxAngularVelocity / AngularVelocityMagnitude;
			}
			else
			{
				AngularVelocity = FVector::ZeroVector;
			}
		}

		// UE_LOG(LogTemp, Log, TEXT("AngularVelocity %s"), *AngularVelocity.ToString());
	}
}

void UTetherPhysicsSolverAngular::DrawDebug(const FTetherIO* InputData, FTetherIO* OutputData,
	TMap<const FTetherShape*, const FTransform*> ShapeTransforms, TArray<FTetherDebugText>* PendingDebugText,
	float LifeTime,	FAnimInstanceProxy* Proxy, const UWorld* World, const FColor& VelocityColor,
	const FColor& ForceColor, const FColor& AccelerationColor, bool bPersistentLines, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	if (!FTether::CVarTetherSolverAngularDraw.GetValueOnAnyThread())
	{
		return;
	}
	
	// Ensure there is a valid Proxy or World context
	if (!Proxy && !World)
	{
		return;
	}

	const auto* Input = InputData->GetDataIO<FAngularInput>();
	const auto* Output = OutputData->GetDataIO<FAngularOutput>();

	for (auto& ShapeItr : Input->ShapeSettings)
	{
		const FTetherShape* const& Shape = ShapeItr.Key;
		// const FAngularInputSettings& Settings = ShapeItr.Value;
		const FAngularOutputData& Data = Output->ShapeData[Shape];

		// Get the transform for the shape
		FVector Center = ShapeTransforms[Shape]->GetLocation();
		FQuat Rotation = ShapeTransforms[Shape]->GetRotation();

		static constexpr float GizmoRadius = 24.f;
		
		UTetherStatics::DrawRotationGizmo(World, Proxy, Center, Rotation, Data.AngularVelocity, GizmoRadius, 24.f, 32,
			FColor::Yellow, FColor::Red, FColor::Green, FColor::Blue,
			bPersistentLines, LifeTime, Thickness);

		const FVector ArrowEndLocation = Center + Data.AngularVelocity.GetSafeNormal() * GizmoRadius;
		UTetherStatics::DrawText("Angular Velocity", PendingDebugText, Shape, ArrowEndLocation, FColor::Yellow);
	}
#endif
}
