// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_STATS_GROUP(TEXT("Tether_Stat"), STATGROUP_Tether, STATCAT_Advanced); 

DEFINE_LOG_CATEGORY_STATIC(LogTether, Log, All);

namespace FTether
{
	/**
	 * Inertia is a measure of how hard it is to rotate an object, and it depends on both the shape of the object and how
	 * its mass is distributed. For a box, the formula considers the mass of the box and the lengths of its sides.
	 *
	 * 12.0f is a constant that results from the math involved in determining how the mass is spread out relative to the
	 * axis of rotation. It helps correctly scale the influence of the box's dimensions on its rotational inertia.
	 */
	static constexpr float MomentOfInertia = 12.f;
}