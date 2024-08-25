// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShapeCollisionControl.h"

#include "TetherIO.h"
#include "Shapes/TetherShapeCaster.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"
#include "Shapes/TetherShape_BoundingSphere.h"
#include "Shapes/TetherShape_Capsule.h"
#include "Shapes/TetherShape_Cone.h"
#include "Shapes/TetherShape_OrientedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShapeCollisionControl)

FVector UTetherShapeCollisionControl::ClampVector(const FVector& InVector, const FVector& Min, const FVector& Max)
{
	FVector Result;
	Result.X = FMath::Clamp(InVector.X, Min.X, Max.X);
	Result.Y = FMath::Clamp(InVector.Y, Min.Y, Max.Y);
	Result.Z = FMath::Clamp(InVector.Z, Min.Z, Max.Z);
	return Result;
}

bool UTetherShapeCollisionControl::CheckBroadCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB) const
{
	if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Broad_AABB_AABB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Broad_AABB_BoundingSphere(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Broad_AABB_OBB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Broad_AABB_Capsule(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Broad_AABB_Cone(A, B);
		}
	}
	else if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Broad_BoundingSphere_AABB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Broad_BoundingSphere_BoundingSphere(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Broad_BoundingSphere_OBB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Broad_BoundingSphere_Capsule(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Broad_BoundingSphere_Cone(A, B);
		}
	}
	else if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Broad_OBB_AABB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Broad_OBB_BoundingSphere(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Broad_OBB_OBB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Broad_OBB_Capsule(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Broad_OBB_Cone(A, B);
		}
	}
	else if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Broad_Capsule_AABB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Broad_Capsule_BoundingSphere(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Broad_Capsule_OBB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Broad_Capsule_Capsule(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Broad_Capsule_Cone(A, B);
		}
	}
	else if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Broad_Cone_AABB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Broad_Cone_BoundingSphere(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Broad_Cone_OBB(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Broad_Cone_Capsule(A, B);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Broad_Cone_Cone(A, B);
		}
	}

	return false;
}

bool UTetherShapeCollisionControl::CheckNarrowCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB, FTetherNarrowPhaseCollisionOutput& Output) const
{
	if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Narrow_AABB_AABB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Narrow_AABB_BoundingSphere(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Narrow_AABB_OBB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Narrow_AABB_Capsule(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Narrow_AABB_Cone(A, B, Output);
		}
	}
	else if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Narrow_BoundingSphere_AABB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Narrow_BoundingSphere_BoundingSphere(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Narrow_BoundingSphere_OBB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Narrow_BoundingSphere_Capsule(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Narrow_BoundingSphere_Cone(A, B, Output);
		}
	}
	else if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Narrow_OBB_AABB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Narrow_OBB_BoundingSphere(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Narrow_OBB_OBB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Narrow_OBB_Capsule(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Narrow_OBB_Cone(A, B, Output);
		}
	}
	else if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Narrow_Capsule_AABB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Narrow_Capsule_BoundingSphere(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Narrow_Capsule_OBB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Narrow_Capsule_Capsule(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Narrow_Capsule_Cone(A, B, Output);
		}
	}
	else if (ShapeA.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeA);
		if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&ShapeB);
			return Narrow_Cone_AABB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&ShapeB);
			return Narrow_Cone_BoundingSphere(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&ShapeB);
			return Narrow_Cone_OBB(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&ShapeB);
			return Narrow_Cone_Capsule(A, B, Output);
		}
		else if (ShapeB.GetShapeType() == FTetherGameplayTags::Tether_Shape_Cone)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&ShapeB);
			return Narrow_Cone_Cone(A, B, Output);
		}
	}

	return false;
}

// AABB vs AABB
bool UTetherShapeCollisionControl::Broad_AABB_AABB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	constexpr float Tolerance = KINDA_SMALL_NUMBER;

	return (A->Min.X <= B->Max.X + Tolerance && A->Max.X >= B->Min.X - Tolerance) &&
		   (A->Min.Y <= B->Max.Y + Tolerance && A->Max.Y >= B->Min.Y - Tolerance) &&
		   (A->Min.Z <= B->Max.Z + Tolerance && A->Max.Z >= B->Min.Z - Tolerance);
}

// AABB vs BoundingSphere
bool UTetherShapeCollisionControl::Broad_AABB_BoundingSphere(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_BoundingSphere* B)
{
	const FVector ClosestPoint = ClampVector(B->Center, A->Min, A->Max);
	const float DistanceSquared = FVector::DistSquared(ClosestPoint, B->Center);
	return DistanceSquared <= FMath::Square(B->Radius + KINDA_SMALL_NUMBER);
}

// AABB vs OBB
bool UTetherShapeCollisionControl::Broad_AABB_OBB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B)
{
	// Call the corresponding OBB vs AABB function (symmetric)
	return Broad_OBB_AABB(B, A);
}

// AABB vs Capsule
bool UTetherShapeCollisionControl::Broad_AABB_Capsule(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Capsule* B)
{
	// Implement the broad-phase logic for AABB vs Capsule
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(A, &CapsuleAABB);
}

// AABB vs Cone
bool UTetherShapeCollisionControl::Broad_AABB_Cone(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Cone* B)
{
	// Implement the broad-phase logic for AABB vs Cone
	FTetherShape_AxisAlignedBoundingBox ConeAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(A, &ConeAABB);
}

// BoundingSphere vs AABB
bool UTetherShapeCollisionControl::Broad_BoundingSphere_AABB(const FTetherShape_BoundingSphere* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	// Call the corresponding AABB vs BoundingSphere function (symmetric)
	return Broad_AABB_BoundingSphere(B, A);
}

// BoundingSphere vs BoundingSphere
bool UTetherShapeCollisionControl::Broad_BoundingSphere_BoundingSphere(const FTetherShape_BoundingSphere* A, const FTetherShape_BoundingSphere* B)
{
	const float DistanceSquared = FVector::DistSquared(A->Center, B->Center);
	const float RadiusSum = A->Radius + B->Radius;
	return DistanceSquared <= FMath::Square(RadiusSum + KINDA_SMALL_NUMBER);
}

// BoundingSphere vs OBB
bool UTetherShapeCollisionControl::Broad_BoundingSphere_OBB(const FTetherShape_BoundingSphere* A, const FTetherShape_OrientedBoundingBox* B)
{
	// Implement the broad-phase logic for BoundingSphere vs OBB
	FTetherShape_AxisAlignedBoundingBox OBBAABB = B->GetBoundingBox();
	return Broad_BoundingSphere_AABB(A, &OBBAABB);
}

// BoundingSphere vs Capsule
bool UTetherShapeCollisionControl::Broad_BoundingSphere_Capsule(const FTetherShape_BoundingSphere* A, const FTetherShape_Capsule* B)
{
	// Implement the broad-phase logic for BoundingSphere vs Capsule
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = B->GetBoundingBox();
	return Broad_BoundingSphere_AABB(A, &CapsuleAABB);
}

// BoundingSphere vs Cone
bool UTetherShapeCollisionControl::Broad_BoundingSphere_Cone(const FTetherShape_BoundingSphere* A, const FTetherShape_Cone* B)
{
	// Implement the broad-phase logic for BoundingSphere vs Cone
	FTetherShape_AxisAlignedBoundingBox ConeAABB = B->GetBoundingBox();
	return Broad_BoundingSphere_AABB(A, &ConeAABB);
}

// OBB vs AABB
bool UTetherShapeCollisionControl::Broad_OBB_AABB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	// Implement the broad-phase logic for OBB vs AABB
	FTetherShape_AxisAlignedBoundingBox OBBAABB = A->GetBoundingBox();
	return Broad_AABB_AABB(&OBBAABB, B);
}

// OBB vs BoundingSphere
bool UTetherShapeCollisionControl::Broad_OBB_BoundingSphere(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_BoundingSphere* B)
{
	// Implement the broad-phase logic for OBB vs BoundingSphere
	FTetherShape_AxisAlignedBoundingBox OBBAABB = A->GetBoundingBox();
	return Broad_BoundingSphere_AABB(B, &OBBAABB);
}

// OBB vs OBB
bool UTetherShapeCollisionControl::Broad_OBB_OBB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B)
{
	// Implement the broad-phase logic for OBB vs OBB
	FTetherShape_AxisAlignedBoundingBox AABB_A = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox AABB_B = B->GetBoundingBox();
	return Broad_AABB_AABB(&AABB_A, &AABB_B);
}

// OBB vs Capsule
bool UTetherShapeCollisionControl::Broad_OBB_Capsule(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Capsule* B)
{
	// Implement the broad-phase logic for OBB vs Capsule
	FTetherShape_AxisAlignedBoundingBox OBBAABB = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(&OBBAABB, &CapsuleAABB);
}

// OBB vs Cone
bool UTetherShapeCollisionControl::Broad_OBB_Cone(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Cone* B)
{
	// Implement the broad-phase logic for OBB vs Cone
	FTetherShape_AxisAlignedBoundingBox OBBAABB = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox ConeAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(&OBBAABB, &ConeAABB);
}

// Capsule vs AABB
bool UTetherShapeCollisionControl::Broad_Capsule_AABB(const FTetherShape_Capsule* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	// Implement the broad-phase logic for Capsule vs AABB
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = A->GetBoundingBox();
	return Broad_AABB_AABB(&CapsuleAABB, B);
}

// Capsule vs BoundingSphere
bool UTetherShapeCollisionControl::Broad_Capsule_BoundingSphere(const FTetherShape_Capsule* A, const FTetherShape_BoundingSphere* B)
{
	// Implement the broad-phase logic for Capsule vs BoundingSphere
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = A->GetBoundingBox();
	return Broad_BoundingSphere_AABB(B, &CapsuleAABB);
}

// Capsule vs OBB
bool UTetherShapeCollisionControl::Broad_Capsule_OBB(const FTetherShape_Capsule* A, const FTetherShape_OrientedBoundingBox* B)
{
	// Implement the broad-phase logic for Capsule vs OBB
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox OBBAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(&CapsuleAABB, &OBBAABB);
}

// Capsule vs Capsule
bool UTetherShapeCollisionControl::Broad_Capsule_Capsule(const FTetherShape_Capsule* A, const FTetherShape_Capsule* B)
{
	// Implement the broad-phase logic for Capsule vs Capsule
	FTetherShape_AxisAlignedBoundingBox AABB_A = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox AABB_B = B->GetBoundingBox();
	return Broad_AABB_AABB(&AABB_A, &AABB_B);
}

// Capsule vs Cone
bool UTetherShapeCollisionControl::Broad_Capsule_Cone(const FTetherShape_Capsule* A, const FTetherShape_Cone* B)
{
	// Implement the broad-phase logic for Capsule vs Cone
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox ConeAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(&CapsuleAABB, &ConeAABB);
}

// Cone vs AABB
bool UTetherShapeCollisionControl::Broad_Cone_AABB(const FTetherShape_Cone* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	// Implement the broad-phase logic for Cone vs AABB
	FTetherShape_AxisAlignedBoundingBox ConeAABB = A->GetBoundingBox();
	return Broad_AABB_AABB(&ConeAABB, B);
}

// Cone vs BoundingSphere
bool UTetherShapeCollisionControl::Broad_Cone_BoundingSphere(const FTetherShape_Cone* A, const FTetherShape_BoundingSphere* B)
{
	// Implement the broad-phase logic for Cone vs BoundingSphere
	FTetherShape_AxisAlignedBoundingBox ConeAABB = A->GetBoundingBox();
	return Broad_BoundingSphere_AABB(B, &ConeAABB);
}

// Cone vs OBB
bool UTetherShapeCollisionControl::Broad_Cone_OBB(const FTetherShape_Cone* A, const FTetherShape_OrientedBoundingBox* B)
{
	// Implement the broad-phase logic for Cone vs OBB
	FTetherShape_AxisAlignedBoundingBox ConeAABB = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox OBBAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(&ConeAABB, &OBBAABB);
}

// Cone vs Capsule
bool UTetherShapeCollisionControl::Broad_Cone_Capsule(const FTetherShape_Cone* A, const FTetherShape_Capsule* B)
{
	// Implement the broad-phase logic for Cone vs Capsule
	FTetherShape_AxisAlignedBoundingBox ConeAABB = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(&ConeAABB, &CapsuleAABB);
}

// Cone vs Cone
bool UTetherShapeCollisionControl::Broad_Cone_Cone(const FTetherShape_Cone* A, const FTetherShape_Cone* B)
{
	// Implement the broad-phase logic for Cone vs Cone
	FTetherShape_AxisAlignedBoundingBox AABB_A = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox AABB_B = B->GetBoundingBox();
	return Broad_AABB_AABB(&AABB_A, &AABB_B);
}

// Narrow-phase collision check for AABB vs AABB
bool UTetherShapeCollisionControl::Narrow_AABB_AABB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	if (Broad_AABB_AABB(A, B))
	{
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for AABB vs BoundingSphere
bool UTetherShapeCollisionControl::Narrow_AABB_BoundingSphere(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	if (Broad_AABB_BoundingSphere(A, B))
	{
		FVector ClosestPoint = ClampVector(B->Center, A->Min, A->Max);
		float DistanceSquared = FVector::DistSquared(ClosestPoint, B->Center);
		
		if (DistanceSquared <= FMath::Square(B->Radius))
		{
			Output.bHasCollision = true;
			Output.ContactPoint = ClosestPoint;
			Output.PenetrationDepth = B->Radius - FMath::Sqrt(DistanceSquared);
			return true;
		}
	}
	return false;
}

// Narrow-phase collision check for AABB vs OBB
bool UTetherShapeCollisionControl::Narrow_AABB_OBB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for AABB vs OBB
	// Example: SAT (Separating Axis Theorem) can be used for this detailed check.
	// Placeholder logic for now:
	if (Broad_AABB_OBB(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for AABB vs Capsule
bool UTetherShapeCollisionControl::Narrow_AABB_Capsule(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for AABB vs Capsule
	if (Broad_AABB_Capsule(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for AABB vs Cone
bool UTetherShapeCollisionControl::Narrow_AABB_Cone(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for AABB vs Cone
	if (Broad_AABB_Cone(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for BoundingSphere vs AABB
bool UTetherShapeCollisionControl::Narrow_BoundingSphere_AABB(const FTetherShape_BoundingSphere* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_AABB_BoundingSphere(B, A, Output); // Symmetric to AABB vs BoundingSphere
}

// Narrow-phase collision check for BoundingSphere vs BoundingSphere
bool UTetherShapeCollisionControl::Narrow_BoundingSphere_BoundingSphere(const FTetherShape_BoundingSphere* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	if (Broad_BoundingSphere_BoundingSphere(A, B))
	{
		float Distance = FVector::Dist(A->Center, B->Center);
		if (Distance <= A->Radius + B->Radius)
		{
			Output.bHasCollision = true;
			Output.ContactPoint = (A->Center + B->Center) * 0.5f;
			Output.PenetrationDepth = (A->Radius + B->Radius) - Distance;
			return true;
		}
	}
	return false;
}

// Narrow-phase collision check for BoundingSphere vs OBB
bool UTetherShapeCollisionControl::Narrow_BoundingSphere_OBB(const FTetherShape_BoundingSphere* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for BoundingSphere vs OBB
	if (Broad_BoundingSphere_OBB(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->Center + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for BoundingSphere vs Capsule
bool UTetherShapeCollisionControl::Narrow_BoundingSphere_Capsule(const FTetherShape_BoundingSphere* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for BoundingSphere vs Capsule
	if (Broad_BoundingSphere_Capsule(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->Center + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for BoundingSphere vs Cone
bool UTetherShapeCollisionControl::Narrow_BoundingSphere_Cone(const FTetherShape_BoundingSphere* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for BoundingSphere vs Cone
	if (Broad_BoundingSphere_Cone(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->Center + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for OBB vs AABB
bool UTetherShapeCollisionControl::Narrow_OBB_AABB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for OBB vs AABB
	if (Broad_OBB_AABB(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for OBB vs BoundingSphere
bool UTetherShapeCollisionControl::Narrow_OBB_BoundingSphere(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for OBB vs BoundingSphere
	return Narrow_BoundingSphere_OBB(B, A, Output); // Symmetric to BoundingSphere vs OBB
}

// Narrow-phase collision check for OBB vs OBB
bool UTetherShapeCollisionControl::Narrow_OBB_OBB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for OBB vs OBB
	if (Broad_OBB_OBB(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for OBB vs Capsule
bool UTetherShapeCollisionControl::Narrow_OBB_Capsule(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for OBB vs Capsule
	if (Broad_OBB_Capsule(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for OBB vs Cone
bool UTetherShapeCollisionControl::Narrow_OBB_Cone(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for OBB vs Cone
	if (Broad_OBB_Cone(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for Capsule vs AABB
bool UTetherShapeCollisionControl::Narrow_Capsule_AABB(const FTetherShape_Capsule* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_AABB_Capsule(B, A, Output); // Symmetric to AABB vs Capsule
}

// Narrow-phase collision check for Capsule vs BoundingSphere
bool UTetherShapeCollisionControl::Narrow_Capsule_BoundingSphere(const FTetherShape_Capsule* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_BoundingSphere_Capsule(B, A, Output); // Symmetric to BoundingSphere vs Capsule
}

// Narrow-phase collision check for Capsule vs OBB
bool UTetherShapeCollisionControl::Narrow_Capsule_OBB(const FTetherShape_Capsule* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_OBB_Capsule(B, A, Output); // Symmetric to OBB vs Capsule
}

// Narrow-phase collision check for Capsule vs Capsule
bool UTetherShapeCollisionControl::Narrow_Capsule_Capsule(const FTetherShape_Capsule* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	if (Broad_Capsule_Capsule(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for Capsule vs Cone
bool UTetherShapeCollisionControl::Narrow_Capsule_Cone(const FTetherShape_Capsule* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for Capsule vs Cone
	if (Broad_Capsule_Cone(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for Cone vs AABB
bool UTetherShapeCollisionControl::Narrow_Cone_AABB(const FTetherShape_Cone* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_AABB_Cone(B, A, Output); // Symmetric to AABB vs Cone
}

// Narrow-phase collision check for Cone vs BoundingSphere
bool UTetherShapeCollisionControl::Narrow_Cone_BoundingSphere(const FTetherShape_Cone* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_BoundingSphere_Cone(B, A, Output); // Symmetric to BoundingSphere vs Cone
}

// Narrow-phase collision check for Cone vs OBB
bool UTetherShapeCollisionControl::Narrow_Cone_OBB(const FTetherShape_Cone* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_OBB_Cone(B, A, Output); // Symmetric to OBB vs Cone
}

// Narrow-phase collision check for Cone vs Capsule
bool UTetherShapeCollisionControl::Narrow_Cone_Capsule(const FTetherShape_Cone* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_Capsule_Cone(B, A, Output); // Symmetric to Capsule vs Cone
}

// Narrow-phase collision check for Cone vs Cone
bool UTetherShapeCollisionControl::Narrow_Cone_Cone(const FTetherShape_Cone* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	if (Broad_Cone_Cone(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.bHasCollision = true;
		Output.ContactPoint = (A->GetCenter() + B->GetCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}
