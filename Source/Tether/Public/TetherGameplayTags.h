// Copyright (c) Jared Taylor. All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

namespace FTetherGameplayTags
{
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver);
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Physics);
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Physics_Linear);
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Physics_Angular);
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Solver_Contact);
	
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape);
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_AxisAlignedBoundingBox);
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_OrientedBoundingBox);
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_BoundingSphere);
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_Capsule);
	TETHER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tether_Shape_Cone);
}