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

void UTetherPhysicsSolverLinear::ApplyLinearDamping(FVector& Velocity, const FLinearInputSettings& Settings,
	float DeltaTime)
{
	// Apply damping to slow down linear velocity
	switch (Settings.DampingModel)
	{
	case ETetherDampingModel::SimpleLinear:
		Velocity *= (1.f - Settings.LinearDamping * DeltaTime);
		break;
	case ETetherDampingModel::ExponentialDecay:
		Velocity *= FMath::Exp(-Settings.LinearDamping * DeltaTime);
		break;
	}
}

void UTetherPhysicsSolverLinear::Solve(FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData,
	float DeltaTime, float WorldTime) const
{
	const auto* Input = InputData->GetDataIO<FLinearInput>();
	auto* Output = OutputData->GetDataIO<FLinearOutput>();

	// Skip asleep shapes
	if (Shape->IsAsleep())
	{
		return;
	}

	// Skip Kinematic objects - they don't respond to physics forces
	if (Shape->SimulationMode == ETetherSimulationMode::Kinematic)
	{
		return;
	}
	
	const FLinearInputSettings& Settings = Input->Settings;
	FVector& LinearVelocity = Output->LinearVelocity;

	// Inertial mode only applies damping, no external forces
	if (Shape->SimulationMode == ETetherSimulationMode::Inertial)
	{
		ApplyLinearDamping(LinearVelocity, Settings, DeltaTime);
		return;
	}

	// Simulated mode: Apply forces, acceleration, and damping
	const float Mass = FMath::Max(KINDA_SMALL_NUMBER, Settings.Mass);
	FVector Acceleration = (Settings.Force - Settings.FrictionForce) / Mass;
	Acceleration += Settings.Acceleration;

	// Apply Acceleration to the Linear Velocity
	LinearVelocity += Acceleration * DeltaTime;

	// Apply damping to slow down linear velocity
	ApplyLinearDamping(LinearVelocity, Settings, DeltaTime);

	// Apply linear drag due to air resistance
	float DragForce = Settings.LinearDragCoefficient * LinearVelocity.SizeSquared();
	FVector Drag = -LinearVelocity.GetSafeNormal() * DragForce * DeltaTime;
	LinearVelocity += Drag;

	// Clamp the linear velocity to the maximum allowed value
	float LinearVelocityMagnitude = LinearVelocity.Size();
	if (LinearVelocityMagnitude > Settings.MaxLinearVelocity)
	{
		LinearVelocity *= Settings.MaxLinearVelocity / LinearVelocityMagnitude;
	}

	// UE_LOG(LogTemp, Log, TEXT("LinearVelocity %s"), *LinearVelocity.ToString());
}

void UTetherPhysicsSolverLinear::DrawDebug(const FTetherShape* Shape, const FTetherIO* InputData, FTetherIO* OutputData,
	TArray<FTetherDebugText>* PendingDebugText, float LifeTime, FAnimInstanceProxy* Proxy, const UWorld* World,
	const FColor& VelocityColor, const FColor& ForceColor, const FColor& AccelerationColor,
	bool bPersistentLines, float Thickness) const
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

	const FLinearInputSettings& Settings = Input->Settings;

	static constexpr float VisualScale = 0.05f;
	static constexpr float ArrowSize = 4.f;
	
	// 1. Draw Linear Velocity as an arrow
	FVector StartLocation = Shape->GetAppliedWorldTransform().GetLocation();
	FVector EndLocation = StartLocation + Output->LinearVelocity * VisualScale;  // Scale velocity for visualization
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
#endif
}
