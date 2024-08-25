// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShapeCollisionControl.h"

#include "TetherIO.h"
#include "TetherStatics.h"
#include "Shapes/TetherShapeTypeCaster.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShapeCollisionControl)

bool UTetherShapeCollisionControl::CheckBroadCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB) const
{
	if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
	{
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* A = FTetherShapeTypeCaster::CastShapeChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeA);
			const auto* B = FTetherShapeTypeCaster::CastShapeChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeA);
			return Broad_AABB_AABB(A, B);
		}
	}

	return false;
}

bool UTetherShapeCollisionControl::CheckNarrowCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB, FTetherNarrowPhaseCollisionOutput& Output) const
{
	if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
	{
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* A = FTetherShapeTypeCaster::CastShapeChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeA);
			const auto* B = FTetherShapeTypeCaster::CastShapeChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeA);
			if (Broad_AABB_AABB(A, B))
			{
				Output.bHasCollision = true;
				// Use the midpoint between the two AABBs as a simple contact point
				Output.ContactPoint = (ShapeA.GetCenter() + ShapeB.GetCenter()) * 0.5f;
				Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
				return true;
			}
		}
	}
	
	return false;
}

bool UTetherShapeCollisionControl::Broad_AABB_AABB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	constexpr float Tolerance = KINDA_SMALL_NUMBER;

	UE_LOG(LogTemp, Warning, TEXT("AABB A: Min: %s, Max: %s"), *A->Min.ToString(), *A->Max.ToString());
	UE_LOG(LogTemp, Warning, TEXT("AABB B: Min: %s, Max: %s"), *B->Min.ToString(), *B->Max.ToString());

	return (A->Min.X <= B->Max.X + Tolerance && A->Max.X >= B->Min.X - Tolerance) &&
		   (A->Min.Y <= B->Max.Y + Tolerance && A->Max.Y >= B->Min.Y - Tolerance) &&
		   (A->Min.Z <= B->Max.Z + Tolerance && A->Max.Z >= B->Min.Z - Tolerance);
}

