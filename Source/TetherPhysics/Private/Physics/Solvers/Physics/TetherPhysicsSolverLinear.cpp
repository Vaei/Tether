// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Solvers/Physics/TetherPhysicsSolverLinear.h"

#include "Animation/AnimInstanceProxy.h"
#include "System/TetherDrawing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherPhysicsSolverLinear)

namespace FTether
{
#if ENABLE_DRAW_DEBUG
	TAutoConsoleVariable<bool> CVarTetherSolverLinearDraw(TEXT("p.Tether.Solver.Linear.Draw"), false, TEXT("Draw Tether Linear Solver Helpers"));
#endif
}

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

void UTetherPhysicsSolverLinear::DrawDebug(const FTetherIO* InputData, FTetherIO* OutputData,
	TMap<const FTetherShape*, const FTransform*> ShapeTransforms, TArray<FTetherDebugText>* PendingDebugText,
	float LifeTime,	FAnimInstanceProxy* Proxy, const UWorld* World, const FColor& VelocityColor,
	const FColor& ForceColor, const FColor& AccelerationColor, bool bPersistentLines, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	if (!FTether::CVarTetherSolverLinearDraw.GetValueOnAnyThread())
	{
		return;
	}
	
	if (!Proxy && !World)
	{
		return;
	}
	
	const auto* Input = InputData->GetDataIO<FLinearInput>();
	const auto* Output = OutputData->GetDataIO<FLinearOutput>();

	for (auto& ShapeItr : Input->ShapeSettings)
	{
		const FTetherShape* const& Shape = ShapeItr.Key;
		const FLinearInputSettings& Settings = ShapeItr.Value;
		const FLinearOutputData& Data = Output->ShapeData[Shape];

		static constexpr float VisualScale = 0.05f;
		static constexpr float ArrowSize = 4.f;
		
		// 1. Draw Linear Velocity as an arrow
		FVector StartLocation = ShapeTransforms[Shape]->GetLocation();
		FVector EndLocation = StartLocation + Data.LinearVelocity * VisualScale;  // Scale velocity for visualization
		UTetherDrawing::DrawArrow(World, Proxy, StartLocation, EndLocation, VelocityColor, ArrowSize, bPersistentLines, LifeTime, Thickness);
		UTetherDrawing::DrawText("Linear Velocity", PendingDebugText, Shape, EndLocation, VelocityColor);

		// 2. Draw Net Force
		FVector Force = Settings.Force - Settings.FrictionForce;
		FVector ForceEndLocation = StartLocation + Force * VisualScale;  // Scale force for visualization
		UTetherDrawing::DrawArrow(World, Proxy, StartLocation, ForceEndLocation, ForceColor, ArrowSize, bPersistentLines, LifeTime, Thickness);
		UTetherDrawing::DrawText("Force", PendingDebugText, Shape, ForceEndLocation, ForceColor);

		// 3. Draw Acceleration
		FVector AccelerationEndLocation = StartLocation + Settings.Acceleration * VisualScale;  // Scale acceleration for visualization
		UTetherDrawing::DrawArrow(World, Proxy, StartLocation, AccelerationEndLocation, AccelerationColor, ArrowSize, bPersistentLines, LifeTime, Thickness);
		UTetherDrawing::DrawText("Acceleration", PendingDebugText, Shape, AccelerationEndLocation, AccelerationColor);
	}
#endif
}
