// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Collision/TetherCollisionDetectionHandler.h"

#include "TetherIO.h"
#include "Shapes/TetherShapeCaster.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"
#include "Shapes/TetherShape_BoundingSphere.h"
#include "Shapes/TetherShape_Capsule.h"
#include "Shapes/TetherShape_OrientedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherCollisionDetectionHandler)

FVector UTetherCollisionDetectionHandler::ClampVector(const FVector& InVector, const FVector& Min, const FVector& Max)
{
	FVector Result;
	Result.X = FMath::Clamp(InVector.X, Min.X, Max.X);
	Result.Y = FMath::Clamp(InVector.Y, Min.Y, Max.Y);
	Result.Z = FMath::Clamp(InVector.Z, Min.Z, Max.Z);
	return Result;
}

bool UTetherCollisionDetectionHandler::CheckBroadCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB) const
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
	}

	return false;
}

bool UTetherCollisionDetectionHandler::CheckNarrowCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB, FTetherNarrowPhaseCollisionOutput& Output) const
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
	}

	return false;
}

// AABB vs AABB
bool UTetherCollisionDetectionHandler::Broad_AABB_AABB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	constexpr float Tolerance = KINDA_SMALL_NUMBER;

	return (A->Min.X <= B->Max.X + Tolerance && A->Max.X >= B->Min.X - Tolerance) &&
		   (A->Min.Y <= B->Max.Y + Tolerance && A->Max.Y >= B->Min.Y - Tolerance) &&
		   (A->Min.Z <= B->Max.Z + Tolerance && A->Max.Z >= B->Min.Z - Tolerance);
}

// AABB vs BoundingSphere
bool UTetherCollisionDetectionHandler::Broad_AABB_BoundingSphere(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_BoundingSphere* B)
{
	const FVector ClosestPoint = ClampVector(B->Center, A->Min, A->Max);
	const float DistanceSquared = FVector::DistSquared(ClosestPoint, B->Center);
	return DistanceSquared <= FMath::Square(B->Radius + KINDA_SMALL_NUMBER);
}

// AABB vs OBB
bool UTetherCollisionDetectionHandler::Broad_AABB_OBB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B)
{
	// Call the corresponding OBB vs AABB function (symmetric)
	FTetherShape_AxisAlignedBoundingBox OBBAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(A, &OBBAABB);
}

// AABB vs Capsule
bool UTetherCollisionDetectionHandler::Broad_AABB_Capsule(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Capsule* B)
{
	// Implement the broad-phase logic for AABB vs Capsule
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(A, &CapsuleAABB);
}

// BoundingSphere vs AABB
bool UTetherCollisionDetectionHandler::Broad_BoundingSphere_AABB(const FTetherShape_BoundingSphere* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	// Call the corresponding AABB vs BoundingSphere function (symmetric)
	return Broad_AABB_BoundingSphere(B, A);
}

// BoundingSphere vs BoundingSphere
bool UTetherCollisionDetectionHandler::Broad_BoundingSphere_BoundingSphere(const FTetherShape_BoundingSphere* A, const FTetherShape_BoundingSphere* B)
{
	const float DistanceSquared = FVector::DistSquared(A->Center, B->Center);
	const float RadiusSum = A->Radius + B->Radius;
	return DistanceSquared <= FMath::Square(RadiusSum + KINDA_SMALL_NUMBER);
}

// BoundingSphere vs OBB
bool UTetherCollisionDetectionHandler::Broad_BoundingSphere_OBB(const FTetherShape_BoundingSphere* A, const FTetherShape_OrientedBoundingBox* B)
{
	// Reuse the logic from Broad_OBB_BoundingSphere since BoundingSphere vs OBB and OBB vs BoundingSphere are symmetrical
	return Broad_OBB_BoundingSphere(B, A);
}

// BoundingSphere vs Capsule
bool UTetherCollisionDetectionHandler::Broad_BoundingSphere_Capsule(const FTetherShape_BoundingSphere* A, const FTetherShape_Capsule* B)
{
	// Implement the broad-phase logic for BoundingSphere vs Capsule
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = B->GetBoundingBox();
	return Broad_BoundingSphere_AABB(A, &CapsuleAABB);
}

// OBB vs AABB
bool UTetherCollisionDetectionHandler::Broad_OBB_AABB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	// Reuse the logic from Broad_OBB_AABB since AABB vs OBB and OBB vs AABB are symmetrical
	return Broad_AABB_OBB(B, A);
}

// OBB vs BoundingSphere
bool UTetherCollisionDetectionHandler::Broad_OBB_BoundingSphere(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_BoundingSphere* B)
{
	// Implement the broad-phase logic for OBB vs BoundingSphere
	FTetherShape_AxisAlignedBoundingBox OBBAABB = A->GetBoundingBox();
	return Broad_BoundingSphere_AABB(B, &OBBAABB);
}

// OBB vs OBB
bool UTetherCollisionDetectionHandler::Broad_OBB_OBB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B)
{
	// Implement the broad-phase logic for OBB vs OBB
	FTetherShape_AxisAlignedBoundingBox AABB_A = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox AABB_B = B->GetBoundingBox();
	return Broad_AABB_AABB(&AABB_A, &AABB_B);
}

// OBB vs Capsule
bool UTetherCollisionDetectionHandler::Broad_OBB_Capsule(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Capsule* B)
{
	// Reuse the logic from Broad_Capsule_OBB since OBB vs Capsule and Capsule vs OBB are symmetrical
	return Broad_Capsule_OBB(B, A);
}

// Capsule vs AABB
bool UTetherCollisionDetectionHandler::Broad_Capsule_AABB(const FTetherShape_Capsule* A, const FTetherShape_AxisAlignedBoundingBox* B)
{
	// Implement the broad-phase logic for Capsule vs AABB
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = A->GetBoundingBox();
	return Broad_AABB_AABB(&CapsuleAABB, B);
}

// Capsule vs BoundingSphere
bool UTetherCollisionDetectionHandler::Broad_Capsule_BoundingSphere(const FTetherShape_Capsule* A, const FTetherShape_BoundingSphere* B)
{
	// Implement the broad-phase logic for Capsule vs BoundingSphere
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = A->GetBoundingBox();
	return Broad_BoundingSphere_AABB(B, &CapsuleAABB);
}

// Capsule vs OBB
bool UTetherCollisionDetectionHandler::Broad_Capsule_OBB(const FTetherShape_Capsule* A, const FTetherShape_OrientedBoundingBox* B)
{
	// Implement the broad-phase logic for Capsule vs OBB
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox OBBAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(&CapsuleAABB, &OBBAABB);
}

// Capsule vs Capsule
bool UTetherCollisionDetectionHandler::Broad_Capsule_Capsule(const FTetherShape_Capsule* A, const FTetherShape_Capsule* B)
{
	// Implement the broad-phase logic for Capsule vs Capsule
	FTetherShape_AxisAlignedBoundingBox AABB_A = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox AABB_B = B->GetBoundingBox();
	return Broad_AABB_AABB(&AABB_A, &AABB_B);
}

// Narrow-phase collision check for AABB vs AABB
bool UTetherCollisionDetectionHandler::Narrow_AABB_AABB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
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
bool UTetherCollisionDetectionHandler::Narrow_AABB_BoundingSphere(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output)
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
bool UTetherCollisionDetectionHandler::Narrow_AABB_OBB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Reuse the logic from Narrow_OBB_AABB since AABB vs OBB and OBB vs AABB are symmetrical
	return Narrow_OBB_AABB(B, A, Output);
}

bool UTetherCollisionDetectionHandler::Narrow_AABB_Capsule(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Get the top and bottom points of the capsule
	FVector CapsuleTop = B->Center + B->Rotation.RotateVector(FVector::UpVector) * (B->HalfHeight - B->Radius);
	FVector CapsuleBottom = B->Center - B->Rotation.RotateVector(FVector::UpVector) * (B->HalfHeight - B->Radius);

	// Get the closest point on the AABB to the capsule's line segment
	FVector ClosestPointOnAABB = ClampVector(B->Center, A->Min, A->Max);

	// Find the closest point on the capsule's segment to the AABB
	FVector ClosestPointOnCapsule;
	FMath::SegmentDistToSegmentSafe(CapsuleBottom, CapsuleTop, ClosestPointOnAABB, ClosestPointOnAABB, ClosestPointOnCapsule, ClosestPointOnAABB);

	// Calculate the distance between the closest points
	float DistanceSquared = FVector::DistSquared(ClosestPointOnCapsule, ClosestPointOnAABB);

	// Check if the distance is less than the radius of the capsule
	if (DistanceSquared <= FMath::Square(B->Radius))
	{
		Output.bHasCollision = true;
		Output.ContactPoint = (ClosestPointOnCapsule + ClosestPointOnAABB) * 0.5f;
		Output.PenetrationDepth = B->Radius - FMath::Sqrt(DistanceSquared);
		return true;
	}

	return false;
}

// Narrow-phase collision check for BoundingSphere vs AABB
bool UTetherCollisionDetectionHandler::Narrow_BoundingSphere_AABB(const FTetherShape_BoundingSphere* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_AABB_BoundingSphere(B, A, Output); // Symmetric to AABB vs BoundingSphere
}

// Narrow-phase collision check for BoundingSphere vs BoundingSphere
bool UTetherCollisionDetectionHandler::Narrow_BoundingSphere_BoundingSphere(const FTetherShape_BoundingSphere* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output)
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
bool UTetherCollisionDetectionHandler::Narrow_BoundingSphere_OBB(const FTetherShape_BoundingSphere* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
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
bool UTetherCollisionDetectionHandler::Narrow_BoundingSphere_Capsule(const FTetherShape_BoundingSphere* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output)
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

// Narrow-phase collision check for OBB vs AABB
bool UTetherCollisionDetectionHandler::Narrow_OBB_AABB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
    // Get the center and half-extents of the AABB
    FVector BCenter = B->GetCenter();
    FVector BExtents = (B->Max - B->Min) * 0.5f;

    // Get the local axes of the OBB
    FQuat ARotation = A->Rotation.Quaternion();
    FVector AX = ARotation.GetAxisX();
    FVector AY = ARotation.GetAxisY();
    FVector AZ = ARotation.GetAxisZ();

    FVector ACenter = A->Center;
    FVector AExtents = A->Extent;

    // 1. Test the axes of the AABB (which are aligned with the world axes)
    FVector AABBAxes[] = { FVector::ForwardVector, FVector::RightVector, FVector::UpVector };

    for (const FVector& Axis : AABBAxes)
    {
        float AABBProjection = FVector::DotProduct(BExtents, Axis.GetAbs());
        float OBBProjection = FMath::Abs(FVector::DotProduct(AExtents.X * AX, Axis)) +
                              FMath::Abs(FVector::DotProduct(AExtents.Y * AY, Axis)) +
                              FMath::Abs(FVector::DotProduct(AExtents.Z * AZ, Axis));

        float Distance = FMath::Abs(FVector::DotProduct(BCenter - ACenter, Axis));

        if (Distance > (AABBProjection + OBBProjection))
        {
            return false; // No collision
        }
    }

    // 2. Test the axes of the OBB
    FVector OBBAxes[] = { AX, AY, AZ };

    for (const FVector& Axis : OBBAxes)
    {
        float AABBProjection = FVector::DotProduct(BExtents, Axis.GetAbs());
        float OBBProjection = FMath::Abs(FVector::DotProduct(AExtents.X * AX, Axis)) +
                              FMath::Abs(FVector::DotProduct(AExtents.Y * AY, Axis)) +
                              FMath::Abs(FVector::DotProduct(AExtents.Z * AZ, Axis));

        float Distance = FMath::Abs(FVector::DotProduct(BCenter - ACenter, Axis));

        if (Distance > (AABBProjection + OBBProjection))
        {
            return false; // No collision
        }
    }

    // If all axes overlap
    Output.bHasCollision = true;
    Output.ContactPoint = (ACenter + BCenter) * 0.5f; // Simplified contact point
    Output.PenetrationDepth = 0.0f; // Placeholder; refine based on actual overlap calculations

    return true;
}

// Narrow-phase collision check for OBB vs BoundingSphere
bool UTetherCollisionDetectionHandler::Narrow_OBB_BoundingSphere(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	// Implement a more precise narrow-phase check for OBB vs BoundingSphere
	return Narrow_BoundingSphere_OBB(B, A, Output); // Symmetric to BoundingSphere vs OBB
}

// Narrow-phase collision check for OBB vs OBB
bool UTetherCollisionDetectionHandler::Narrow_OBB_OBB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
    FVector ACenter = A->Center;
    FVector AExtents = A->Extent;
    FVector BCenter = B->Center;
    FVector BExtents = B->Extent;

    FQuat ARotation = A->Rotation.Quaternion();
    FQuat BRotation = B->Rotation.Quaternion();

    // Get the local axes of both OBBs
    FVector AX = ARotation.GetAxisX();
    FVector AY = ARotation.GetAxisY();
    FVector AZ = ARotation.GetAxisZ();

    FVector BX = BRotation.GetAxisX();
    FVector BY = BRotation.GetAxisY();
    FVector BZ = BRotation.GetAxisZ();

    // Collect all potential separating axes
    FVector Axes[15];
    Axes[0] = AX;
    Axes[1] = AY;
    Axes[2] = AZ;
    Axes[3] = BX;
    Axes[4] = BY;
    Axes[5] = BZ;
    Axes[6] = FVector::CrossProduct(AX, BX);
    Axes[7] = FVector::CrossProduct(AX, BY);
    Axes[8] = FVector::CrossProduct(AX, BZ);
    Axes[9] = FVector::CrossProduct(AY, BX);
    Axes[10] = FVector::CrossProduct(AY, BY);
    Axes[11] = FVector::CrossProduct(AY, BZ);
    Axes[12] = FVector::CrossProduct(AZ, BX);
    Axes[13] = FVector::CrossProduct(AZ, BY);
    Axes[14] = FVector::CrossProduct(AZ, BZ);

    for (const FVector& Axis : Axes)
    {
        if (Axis.IsNearlyZero()) continue;  // Skip near-degenerate axes

        FVector NormalizedAxis = Axis.GetSafeNormal();

        // Project both OBBs onto the axis
        float AProjection = 
            FMath::Abs(FVector::DotProduct(AX * AExtents.X, NormalizedAxis)) +
            FMath::Abs(FVector::DotProduct(AY * AExtents.Y, NormalizedAxis)) +
            FMath::Abs(FVector::DotProduct(AZ * AExtents.Z, NormalizedAxis));
        
        float BProjection = 
            FMath::Abs(FVector::DotProduct(BX * BExtents.X, NormalizedAxis)) +
            FMath::Abs(FVector::DotProduct(BY * BExtents.Y, NormalizedAxis)) +
            FMath::Abs(FVector::DotProduct(BZ * BExtents.Z, NormalizedAxis));

        float Distance = FMath::Abs(FVector::DotProduct(BCenter - ACenter, NormalizedAxis));

        if (Distance > (AProjection + BProjection))
        {
            return false; // No collision
        }
    }

    // If all axes overlap
    Output.bHasCollision = true;
    Output.ContactPoint = (ACenter + BCenter) * 0.5f; // Simplified contact point
    Output.PenetrationDepth = 0.0f; // Placeholder; refine based on actual overlap calculations

    return true;
}

// Narrow-phase collision check for OBB vs Capsule
bool UTetherCollisionDetectionHandler::Narrow_OBB_Capsule(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output)
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

// Narrow-phase collision check for Capsule vs AABB
bool UTetherCollisionDetectionHandler::Narrow_Capsule_AABB(const FTetherShape_Capsule* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_AABB_Capsule(B, A, Output); // Symmetric to AABB vs Capsule
}

// Narrow-phase collision check for Capsule vs BoundingSphere
bool UTetherCollisionDetectionHandler::Narrow_Capsule_BoundingSphere(const FTetherShape_Capsule* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_BoundingSphere_Capsule(B, A, Output); // Symmetric to BoundingSphere vs Capsule
}

// Narrow-phase collision check for Capsule vs OBB
bool UTetherCollisionDetectionHandler::Narrow_Capsule_OBB(const FTetherShape_Capsule* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output)
{
	return Narrow_OBB_Capsule(B, A, Output); // Symmetric to OBB vs Capsule
}

bool UTetherCollisionDetectionHandler::Narrow_Capsule_Capsule(const FTetherShape_Capsule* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output)
{
    // Calculate the top and bottom points of Capsule A (including hemispheres)
    FVector A_Top = A->Center + A->Rotation.RotateVector(FVector::UpVector) * (A->HalfHeight - A->Radius);  // The half height goes to the extent, not the center of the hemisphere!
    FVector A_Bottom = A->Center - A->Rotation.RotateVector(FVector::UpVector) * (A->HalfHeight - A->Radius);

    // Log the calculated positions for debugging
    // UE_LOG(LogTether, Warning, TEXT("Capsule A Top: %s"), *A_Top.ToString());
    // UE_LOG(LogTether, Warning, TEXT("Capsule A Bottom: %s"), *A_Bottom.ToString());

    // Calculate the top and bottom points of Capsule B (including hemispheres)
    FVector B_Top = B->Center + B->Rotation.RotateVector(FVector::UpVector) * (B->HalfHeight - B->Radius);
    FVector B_Bottom = B->Center - B->Rotation.RotateVector(FVector::UpVector) * (B->HalfHeight - B->Radius);

    // Log the calculated positions for debugging
    // UE_LOG(LogTether, Warning, TEXT("Capsule B Top: %s"), *B_Top.ToString());
    // UE_LOG(LogTether, Warning, TEXT("Capsule B Bottom: %s"), *B_Bottom.ToString());

    // Find the closest points between the line segments representing the capsules
    FVector ClosestPointA, ClosestPointB;
    FMath::SegmentDistToSegmentSafe(A_Bottom, A_Top, B_Bottom, B_Top, ClosestPointA, ClosestPointB);

    // Log the closest points for debugging
    // UE_LOG(LogTether, Warning, TEXT("Closest Point A: %s"), *ClosestPointA.ToString());
    // UE_LOG(LogTether, Warning, TEXT("Closest Point B: %s"), *ClosestPointB.ToString());

    // Compute the distance between the closest points
    float DistanceSquared = FVector::DistSquared(ClosestPointA, ClosestPointB);
    float CombinedRadii = A->Radius + B->Radius;

    // Log the distance and combined radii for debugging
    // UE_LOG(LogTether, Warning, TEXT("Distance Squared: %f"), DistanceSquared);
    // UE_LOG(LogTether, Warning, TEXT("Combined Radii: %f"), CombinedRadii);

    // Check if the capsules overlap considering the hemispheres
    if (DistanceSquared <= FMath::Square(CombinedRadii))
    {
        Output.bHasCollision = true;
        Output.ContactPoint = (ClosestPointA + ClosestPointB) * 0.5f;
        Output.PenetrationDepth = CombinedRadii - FMath::Sqrt(DistanceSquared);

        // Log collision detection
        // UE_LOG(LogTether, Warning, TEXT("Collision detected. Contact Point: %s, Penetration Depth: %f"), *Output.ContactPoint.ToString(), Output.PenetrationDepth);

        return true;
    }

    // Log that no collision was detected
    // UE_LOG(LogTether, Warning, TEXT("No collision detected."));
    return false;
}
