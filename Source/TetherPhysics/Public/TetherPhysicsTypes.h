// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherPhysicsTypes.generated.h"

/**
 * Defines how a physics object is controlled within the Tether physics engine.
 */
UENUM(BlueprintType)
enum class ETetherSimulationMode : uint8
{
	Simulated			UMETA(ToolTip="Simulated by the physics engine"),
	Kinematic			UMETA(ToolTip="Does not respond to physics forces"),
	Inertial			UMETA(ToolTip="Retains internal physics like damping but won't apply external forces"),
};

/**
 * Defines the wake/sleep state of a physics object, determining whether it's actively simulated or idle.
 */
UENUM(BlueprintType)
enum class ETetherActivityState : uint8
{
	Awake				UMETA(ToolTip="Awake and actively simulated"),
	ForceAwake			UMETA(ToolTip="Awake and actively simulated - will not sleep"),
	Asleep				UMETA(ToolTip="Asleep and not simulated until disturbed"),
	ForceAsleep			UMETA(ToolTip="Asleep and not simulated even if disturbed - will not wake"),
};

/**
 * Configuration settings for controlling the sleep and wake behavior of physics objects in the Tether physics engine.
 * This struct defines various thresholds for linear and angular forces, acceleration, velocity, and collision activity,
 * which determine when an object should transition between awake and asleep states. 
 * The thresholds help reduce unnecessary computations by allowing objects to sleep when they are at rest or unaffected 
 * by significant forces, while ensuring they wake up when external influences are applied.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherActivitySettings
{
	GENERATED_BODY()

	FTetherActivitySettings()
		: RecentBroadPhaseCollisionTime(1.f)
		, LinearVelocityThreshold(KINDA_SMALL_NUMBER)
		, LinearForceThreshold(0.2f)
		, LinearAccelerationThreshold(0.1f)
		, AngularVelocityThreshold(0.1f)
		, AngularTorqueThreshold(1.f)
		, EnergyThreshold(0.05f)
		, SleepDelay(1.f)
	{}
	
	/**
	 * Time threshold (in seconds) for detecting recent broad-phase collisions. 
	 * If a broad-phase collision occurred within this time window, the object will wake up.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float RecentBroadPhaseCollisionTime;
	
	/**
	 * Minimum linear velocity threshold to keep the object awake.
	 * If the object's linear velocity falls below this threshold, it will be considered for sleeping.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float LinearVelocityThreshold;
	
	/**
	 * Linear force threshold (Newtons) to wake the object. 
	 * If the force applied to the object exceeds this threshold, it will wake up.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float LinearForceThreshold;
	
	/**
	 * Linear acceleration threshold (cm/s²) to wake the object. 
	 * If the object experiences an acceleration higher than this value, it will wake up.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float LinearAccelerationThreshold;
	
	/**
	 * Angular velocity threshold (radians/second) to wake the object. 
	 * If the object's angular velocity exceeds this threshold, it will wake up.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float AngularVelocityThreshold;
	
	/**
	 * Angular torque threshold (Newton-meters) to wake the object. 
	 * If the torque applied to the object exceeds this threshold, it will wake up.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float AngularTorqueThreshold;
	
	/**
	 * Minimum kinetic energy threshold to transition an object to sleep.
	 * If the combined linear and angular kinetic energy of the object falls below this threshold,
	 * the object will be considered for sleep. A lower value makes the object more sensitive
	 * to small movements, while a higher value requires more energy for the object to stay awake.
	 *
	 * Suggestions:
	 * For small props or lightweight objects:			 0.01f
	 * For mid-sized objects (vehicles, characters):	 0.05f
	 * For larger heavier objects:						 0.10f
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float EnergyThreshold;

	/** When entering sleep state, delay for this amount of time, and reset the delay if we should wake */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float SleepDelay;
};