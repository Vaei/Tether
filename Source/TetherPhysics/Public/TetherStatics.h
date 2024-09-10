// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TetherStatics.generated.h"

/**
 * Declares a custom stats group for the Tether system, used for profiling and performance tracking.
 * 
 * This allows you to run "stat tether" to gather and view performance data related to the Tether system within
 * Unreal Engine's profiling tools.
 */
DECLARE_STATS_GROUP(TEXT("Tether_Stat"), STATGROUP_Tether, STATCAT_Advanced); 

/** Logging of messages related to Tether. */
DEFINE_LOG_CATEGORY_STATIC(LogTether, Log, All);

/** Visual logging related to Tether Replay. */
DEFINE_LOG_CATEGORY_STATIC(LogTetherReplay, Log, All);

namespace FTether
{
	/**
	 * The moment of inertia is a measure of an object's resistance to rotation, influenced by its shape and mass distribution.
	 *
	 * For a box-shaped object, the moment of inertia is calculated using the box's mass and its dimensions. The constant
	 * 12.0f is derived from the mathematical formula used to determine how the mass is distributed relative to the axis of rotation.
	 * This constant ensures that the influence of the box's dimensions on its rotational inertia is correctly scaled.
	 */
	static constexpr float MomentOfInertia = 12.f;
}

UCLASS()
class TETHERPHYSICS_API UTetherStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FVector GetVelocityAtPoint(const FVector& Point, const FVector& CenterOfMass, const FVector& LinearVelocity, const FVector& AngularVelocity)
	{
		// Compute the vector from the center of mass to the given point
		const FVector r = Point - CenterOfMass;

		// Calculate the velocity at the point, using the linear and angular velocity
		// The angular contribution is given by AngularVelocity x r
		return LinearVelocity + FVector::CrossProduct(AngularVelocity, r);
	}
};