// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShapeCollisionControl.h"

#include "Shapes/TetherShapeTypeCaster.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShapeCollisionControl)

bool UTetherShapeCollisionControl::CheckBroadCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB) const
{
	if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
	{
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto& A = FTetherShapeTypeCaster::CastShapeChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeA);
			const auto& B = FTetherShapeTypeCaster::CastShapeChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeA);
			return AABB_AABB(A, B);
		}
	}

	return false;
}

bool UTetherShapeCollisionControl::CheckNarrowCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB) const
{
	return false;
}

bool UTetherShapeCollisionControl::AABB_AABB(const FTetherShape_AxisAlignedBoundingBox& A,
	const FTetherShape_AxisAlignedBoundingBox& B)
{
	return (A.Min.X <= B.Max.X && A.Max.X >= B.Min.X) &&
		(A.Min.Y <= B.Max.Y && A.Max.Y >= B.Min.Y) &&
		(A.Min.Z <= B.Max.Z && A.Max.Z >= B.Min.Z);
}
