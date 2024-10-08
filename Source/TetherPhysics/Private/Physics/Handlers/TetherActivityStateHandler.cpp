﻿// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Handlers/TetherActivityStateHandler.h"

#include "TetherSettings.h"
#include "TetherStatics.h"
#include "System/TetherDrawing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherActivityStateHandler)

namespace FTether
{
#if ENABLE_DRAW_DEBUG
	TAutoConsoleVariable<bool> CVarTetherActivityStateLog(TEXT("p.Tether.ActivityState.Log"), false, TEXT("Log Tether Activity State Changes"));
	TAutoConsoleVariable<bool> CVarTetherActivityStateDraw(TEXT("p.Tether.ActivityState.Draw"), false, TEXT("Draw Tether Activity State"));
#endif
}

void UTetherActivityStateHandler::PreSolveWake(FTetherShape* Shape, const FTetherIO* InputData,
	const FTetherIO* LinearInputData, const FTetherIO* AngularInputData, float DeltaTime, double WorldTime) const
{
	const auto* Input = InputData->GetDataIO<FActivityStateInput>();
	const auto* LinearInput = LinearInputData->GetDataIO<FLinearInput>();
	const auto* AngularInput = AngularInputData->GetDataIO<FAngularInput>();

	const FActivitySettings& Settings = Input->Settings;

	// If the object is forcefully asleep, skip further checks and solvers
	if (Shape->ActivityState == ETetherActivityState::ForceAsleep)
	{
		// ForceAsleep means the object is locked in sleep, so do nothing
		return;
	}

	// If the object is currently asleep, check if it should wake up
	if (Shape->ActivityState == ETetherActivityState::Asleep)
	{
		// Wake if recent collision
		if (Shape->HasRecentBroadCollision(WorldTime, Settings.RecentBroadPhaseCollisionTime))
		{
			if (FTether::CVarTetherActivityStateLog.GetValueOnAnyThread())
			{
				UE_LOG(LogTether, Warning, TEXT("{ %s } WOKE due to recent collision"), *Shape->GetName());
			}
			Shape->ActivityState = ETetherActivityState::Awake;
			Shape->TimeUntilSleep = Settings.SleepDelay;
		}
		else
		{
			// Wake the object up due to external linear force, acceleration, or angular torque
			const FLinearInputSettings& LinearSettings = LinearInput->Settings;
			const FAngularInputSettings& AngularSettings = AngularInput->Settings;

			// Check linear force and acceleration thresholds
			const float ForceThresholdSq = FMath::Square(Settings.LinearForceThreshold);
			const float AccelThresholdSq = FMath::Square(Settings.LinearAccelerationThreshold);
			
			// Check angular torque and velocity thresholds
			const float TorqueThresholdSq = FMath::Square(Settings.AngularTorqueThreshold);

			if (LinearSettings.Force.SizeSquared() > ForceThresholdSq ||
				LinearSettings.Acceleration.SizeSquared() > AccelThresholdSq ||
				AngularSettings.Torque.SizeSquared() > TorqueThresholdSq)
			{
				if (FTether::CVarTetherActivityStateLog.GetValueOnAnyThread())
				{
					UE_LOG(LogTether, Warning, TEXT("{ %s } WOKE due to force/acceleration/torque"), *Shape->GetName());
				}
				Shape->ActivityState = ETetherActivityState::Awake;
				Shape->TimeUntilSleep = Settings.SleepDelay;
			}
		}
	}
}

void UTetherActivityStateHandler::PostSolveSleep(FTetherShape* Shape, const FTetherIO* InputData,
	const FTetherIO* LinearInputData, const FTetherIO* AngularInputData, const FTetherIO* LinearOutputData, const
	FTetherIO* AngularOutputData, float	DeltaTime, double WorldTime) const
{
	const auto* Input = InputData->GetDataIO<FActivityStateInput>();
	const auto* LinearInput = LinearInputData->GetDataIO<FLinearInput>();
	const auto* AngularInput = AngularInputData->GetDataIO<FAngularInput>();
	const auto* LinearOutput = LinearOutputData->GetDataIO<FLinearOutput>();
	const auto* AngularOutput = AngularOutputData->GetDataIO<FAngularOutput>();
	
	const FActivitySettings& Settings = Input->Settings;

	// Check if the object is currently awake, and only perform sleep checks if it is
	if (Shape->ActivityState != ETetherActivityState::Awake)
	{
		return;
	}

	// Linear properties
	const FVector& LinearVelocity = LinearOutput->LinearVelocity;
	const float& LinearMass = LinearInput->Settings.Mass;

	// Angular properties
	const float& AngularMass = AngularInput->Settings.Mass;
	const FVector& AngularVelocity = AngularOutput->AngularVelocity;
	const FVector& Inertia = AngularOutput->Inertia;

	// Linear and angular velocity thresholds for sleep
	const float LinearVelocityThresholdSq = FMath::Square(Settings.LinearVelocityThreshold);
	const float AngularVelocityThresholdSq = FMath::Square(Settings.AngularVelocityThreshold);

	// Calculate the total kinetic energy based on linear and angular velocity
	const float LinearKineticEnergy = 0.5f * LinearMass * LinearVelocity.SizeSquared();
	const float AngularKineticEnergy = 0.5f * (AngularVelocity.X * Inertia.X * AngularMass +
										 AngularVelocity.Y * Inertia.Y * AngularMass +
										 AngularVelocity.Z * Inertia.Z * AngularMass);
	float KineticEnergy = LinearKineticEnergy + AngularKineticEnergy;

	if (FTether::CVarTetherActivityStateLog.GetValueOnAnyThread())
	{
		UE_LOG(LogTemp, Verbose, TEXT("{ %s } kinetic energy calculated: LinearKE: %f, AngularKE: %f, TotalKE: %f"), 
			   *Shape->GetName(), LinearKineticEnergy, AngularKineticEnergy, KineticEnergy);
	}
	
	// If the object is moving below velocity thresholds and has low kinetic energy, put it to sleep
	if (LinearVelocity.SizeSquared() < LinearVelocityThresholdSq &&
		AngularVelocity.SizeSquared() < AngularVelocityThresholdSq)
	{
		// Energy-based sleep check
		if (KineticEnergy < Settings.EnergyThreshold)
		{
			// Decrement the sleep timer and only sleep if the timer has elapsed
			Shape->TimeUntilSleep -= DeltaTime;
			if (Shape->TimeUntilSleep <= 0.f)
			{
				// Enter sleep state
				Shape->ActivityState = ETetherActivityState::Asleep;
				if (FTether::CVarTetherActivityStateLog.GetValueOnAnyThread())
				{
					UE_LOG(LogTemp, Warning, TEXT("{ %s } SLEEP due to low kinetic energy"), *Shape->GetName());
				}
			}
		}
	}
	else
	{
		// Reset the sleep timer if the object is still moving significantly
		Shape->TimeUntilSleep = Settings.SleepDelay;
	}
}

void UTetherActivityStateHandler::DrawDebug(FTetherShape* Shape, const FTetherIO* InputData,
	TArray<FTetherDebugText>* PendingDebugText, float LifeTime,
	FAnimInstanceProxy* Proxy, const UWorld* World, const FColor& WakeColor, const FColor& SleepColor,
	bool bPersistentLines, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	if (!FTether::CVarTetherActivityStateDraw.GetValueOnAnyThread())
	{
		return;
	}
	
	const FVector Origin = Shape->GetAppliedWorldTransform().GetLocation() + FVector::UpVector * 5.f;

	const FColor& Color = Shape->IsAwake() ? WakeColor : SleepColor;
	const FString State = UTetherDrawing::GetActivityStateString(Shape->ActivityState);
	
	UTetherDrawing::DrawText(State, PendingDebugText, Shape, Origin, Color);
#endif
}
