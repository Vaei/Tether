﻿// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Physics/Collision/TetherCollisionDetectionHandler.h"

#include "TetherIO.h"
#include "Shapes/TetherShapeCaster.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"
#include "Shapes/TetherShape_BoundingSphere.h"
#include "Shapes/TetherShape_Capsule.h"
#include "Shapes/TetherShape_OrientedBoundingBox.h"
#include "Shapes/TetherShape_Pipe.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherCollisionDetectionHandler)

FVector UTetherCollisionDetectionHandler::ClampVector(const FVector& InVector, const FVector& Min, const FVector& Max)
{
	FVector Result;
	Result.X = FMath::Clamp(InVector.X, Min.X, Max.X);
	Result.Y = FMath::Clamp(InVector.Y, Min.Y, Max.Y);
	Result.Z = FMath::Clamp(InVector.Z, Min.Z, Max.Z);
	return Result;
}

bool UTetherCollisionDetectionHandler::CheckBroadCollision(const FTetherShape* ShapeA, const FTetherShape* ShapeB) const
{
	if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Broad_AABB_AABB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Broad_AABB_BoundingSphere(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Broad_AABB_OBB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Broad_AABB_Capsule(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Broad_AABB_Pipe(A, B);
		}
	}
	else if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Broad_OBB_AABB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Broad_OBB_BoundingSphere(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Broad_OBB_OBB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Broad_OBB_Capsule(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Broad_OBB_Pipe(A, B);
		}
	}
	else if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Broad_BoundingSphere_AABB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Broad_BoundingSphere_BoundingSphere(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Broad_BoundingSphere_OBB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Broad_BoundingSphere_Capsule(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Broad_BoundingSphere_Pipe(A, B);
		}
	}
	else if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Broad_Capsule_AABB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Broad_Capsule_BoundingSphere(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Broad_Capsule_OBB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Broad_Capsule_Capsule(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Broad_Capsule_Pipe(A, B);
		}
	}
	else if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Broad_Pipe_AABB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Broad_Pipe_BoundingSphere(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Broad_Pipe_OBB(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Broad_Pipe_Capsule(A, B);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Broad_Pipe_Pipe(A, B);
		}
	}
	
	return false;
}

bool UTetherCollisionDetectionHandler::CheckNarrowCollision(const FTetherShape* ShapeA, const FTetherShape* ShapeB, FNarrowPhaseCollision& Output) const
{
	if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Narrow_AABB_AABB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Narrow_AABB_BoundingSphere(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Narrow_AABB_OBB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Narrow_AABB_Capsule(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Narrow_AABB_Pipe(A, B, Output);
		}
	}
	else if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Narrow_OBB_AABB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Narrow_OBB_BoundingSphere(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Narrow_OBB_OBB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Narrow_OBB_Capsule(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Narrow_OBB_Pipe(A, B, Output);
		}
	}
	else if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Narrow_BoundingSphere_AABB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Narrow_BoundingSphere_BoundingSphere(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Narrow_BoundingSphere_OBB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Narrow_BoundingSphere_Capsule(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Narrow_BoundingSphere_Pipe(A, B, Output);
		}
	}
	else if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Narrow_Capsule_AABB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Narrow_Capsule_BoundingSphere(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Narrow_Capsule_OBB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Narrow_Capsule_Capsule(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Narrow_Capsule_Pipe(A, B, Output);
		}
	}
	else if (ShapeA->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
	{
		const auto* A = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeA);
		if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(ShapeB);
			return Narrow_Pipe_AABB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_BoundingSphere)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(ShapeB);
			return Narrow_Pipe_BoundingSphere(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(ShapeB);
			return Narrow_Pipe_OBB(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Capsule)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(ShapeB);
			return Narrow_Pipe_Capsule(A, B, Output);
		}
		else if (ShapeB->GetShapeType() == FTetherGameplayTags::Tether_Shape_Pipe)
		{
			const auto* B = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(ShapeB);
			return Narrow_Pipe_Pipe(A, B, Output);
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

// AABB vs Pipe
bool UTetherCollisionDetectionHandler::Broad_AABB_Pipe(const FTetherShape_AxisAlignedBoundingBox* A,
	const FTetherShape_Pipe* B)
{
	// Implement the broad-phase logic for AABB vs Pipe
	FTetherShape_AxisAlignedBoundingBox PipeAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(A, &PipeAABB);
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
	// Calculate the squared distance between the centers of the bounding spheres
	const float DistanceSquared = FVector::DistSquared(A->Center, B->Center);
    
	// Calculate the sum of the radii
	const float RadiusSum = A->Radius + B->Radius;
    
	// Check if the squared distance is less than or equal to the squared sum of the radii
	return DistanceSquared <= FMath::Square(RadiusSum);
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

// BoundingSphere vs Pipe
bool UTetherCollisionDetectionHandler::Broad_BoundingSphere_Pipe(const FTetherShape_BoundingSphere* A,
	const FTetherShape_Pipe* B)
{
	FTetherShape_AxisAlignedBoundingBox PipeAABB = B->GetBoundingBox();
	return Broad_AABB_BoundingSphere(&PipeAABB, A);
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

// OBB vs Pipe
bool UTetherCollisionDetectionHandler::Broad_OBB_Pipe(const FTetherShape_OrientedBoundingBox* A,
	const FTetherShape_Pipe* B)
{
	// Implement the broad-phase logic for OBB vs Pipe
	FTetherShape_AxisAlignedBoundingBox PipeAABB = B->GetBoundingBox();
	return Broad_OBB_AABB(A, &PipeAABB);
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

// Capsule vs Pipe
bool UTetherCollisionDetectionHandler::Broad_Capsule_Pipe(const FTetherShape_Capsule* A, const FTetherShape_Pipe* B)
{
	// Implement the broad-phase logic for Capsule vs Pipe
	FTetherShape_AxisAlignedBoundingBox PipeAABB = B->GetBoundingBox();
	return Broad_Capsule_AABB(A, &PipeAABB);
}

// Pipe vs AABB
bool UTetherCollisionDetectionHandler::Broad_Pipe_AABB(const FTetherShape_Pipe* A,
	const FTetherShape_AxisAlignedBoundingBox* B)
{
	FTetherShape_AxisAlignedBoundingBox PipeAABB = A->GetBoundingBox();
	return Broad_AABB_AABB(&PipeAABB, B);
}

// Pipe vs BoundingSphere
bool UTetherCollisionDetectionHandler::Broad_Pipe_BoundingSphere(const FTetherShape_Pipe* A,
	const FTetherShape_BoundingSphere* B)
{
	FTetherShape_AxisAlignedBoundingBox PipeAABB = A->GetBoundingBox();
	return Broad_AABB_BoundingSphere(&PipeAABB, B);
}

// Pipe vs OBB
bool UTetherCollisionDetectionHandler::Broad_Pipe_OBB(const FTetherShape_Pipe* A,
	const FTetherShape_OrientedBoundingBox* B)
{
	FTetherShape_AxisAlignedBoundingBox PipeAABB = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox OBBAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(&PipeAABB, &OBBAABB);
}

// Pipe vs Capsule
bool UTetherCollisionDetectionHandler::Broad_Pipe_Capsule(const FTetherShape_Pipe* A, const FTetherShape_Capsule* B)
{
	FTetherShape_AxisAlignedBoundingBox PipeAABB = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox CapsuleAABB = B->GetBoundingBox();
	return Broad_AABB_AABB(&PipeAABB, &CapsuleAABB);
}

// Pipe vs Pipe
bool UTetherCollisionDetectionHandler::Broad_Pipe_Pipe(const FTetherShape_Pipe* A, const FTetherShape_Pipe* B)
{
	FTetherShape_AxisAlignedBoundingBox AABB_A = A->GetBoundingBox();
	FTetherShape_AxisAlignedBoundingBox AABB_B = B->GetBoundingBox();
	return Broad_AABB_AABB(&AABB_A, &AABB_B);
}

// Narrow-phase collision check for AABB vs AABB
bool UTetherCollisionDetectionHandler::Narrow_AABB_AABB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B, FNarrowPhaseCollision& Output)
{
	constexpr float Tolerance = KINDA_SMALL_NUMBER;

	// Check for overlap on X, Y, and Z axes
	float OverlapX = FMath::Min(A->Max.X, B->Max.X) - FMath::Max(A->Min.X, B->Min.X);
	float OverlapY = FMath::Min(A->Max.Y, B->Max.Y) - FMath::Max(A->Min.Y, B->Min.Y);
	float OverlapZ = FMath::Min(A->Max.Z, B->Max.Z) - FMath::Max(A->Min.Z, B->Min.Z);

	// If any overlap is less than or equal to zero, no collision occurred
	if (OverlapX <= 0.0f + Tolerance || OverlapY <= 0.0f + Tolerance || OverlapZ <= 0.0f + Tolerance)
	{
		return false; // No collision
	}

	// Calculate the contact point (midpoint between the two centers)
	Output.ContactPoint = (A->GetLocalSpaceCenter() + B->GetLocalSpaceCenter()) * 0.5f;

	// Calculate the penetration depth as the smallest overlap
	Output.PenetrationDepth = FMath::Min(OverlapX, FMath::Min(OverlapY, OverlapZ));

	return true;
}

// Narrow-phase collision check for AABB vs BoundingSphere
bool UTetherCollisionDetectionHandler::Narrow_AABB_BoundingSphere(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_BoundingSphere* B, FNarrowPhaseCollision& Output)
{
	if (Broad_AABB_BoundingSphere(A, B))
	{
		FVector ClosestPoint = ClampVector(B->Center, A->Min, A->Max);
		float DistanceSquared = FVector::DistSquared(ClosestPoint, B->Center);
		
		if (DistanceSquared <= FMath::Square(B->Radius))
		{
			Output.ContactPoint = ClosestPoint;
			Output.PenetrationDepth = B->Radius - FMath::Sqrt(DistanceSquared);
			return true;
		}
	}
	return false;
}

// Narrow-phase collision check for AABB vs OBB
bool UTetherCollisionDetectionHandler::Narrow_AABB_OBB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B, FNarrowPhaseCollision& Output)
{
	// Reuse the logic from Narrow_OBB_AABB since AABB vs OBB and OBB vs AABB are symmetrical
	return Narrow_OBB_AABB(B, A, Output);
}

bool UTetherCollisionDetectionHandler::Narrow_AABB_Capsule(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Capsule* B, FNarrowPhaseCollision& Output)
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
		Output.ContactPoint = (ClosestPointOnCapsule + ClosestPointOnAABB) * 0.5f;
		Output.PenetrationDepth = B->Radius - FMath::Sqrt(DistanceSquared);
		return true;
	}

	return false;
}

// Narrow-phase collision check for Pipe vs AABB
bool UTetherCollisionDetectionHandler::Narrow_AABB_Pipe(const FTetherShape_AxisAlignedBoundingBox* A,
	const FTetherShape_Pipe* B, FNarrowPhaseCollision& Output)
{
	return Narrow_Pipe_AABB(B, A, Output); // Symmetric to Pipe vs AABB
}

// Narrow-phase collision check for BoundingSphere vs AABB
bool UTetherCollisionDetectionHandler::Narrow_BoundingSphere_AABB(const FTetherShape_BoundingSphere* A, const FTetherShape_AxisAlignedBoundingBox* B, FNarrowPhaseCollision& Output)
{
	return Narrow_AABB_BoundingSphere(B, A, Output); // Symmetric to AABB vs BoundingSphere
}

// Narrow-phase collision check for BoundingSphere vs BoundingSphere
bool UTetherCollisionDetectionHandler::Narrow_BoundingSphere_BoundingSphere(const FTetherShape_BoundingSphere* A, const FTetherShape_BoundingSphere* B, FNarrowPhaseCollision& Output)
{
	// Calculate the squared distance between sphere centers
	const float DistanceSquared = FVector::DistSquared(A->Center, B->Center);
	const float RadiusSum = A->Radius + B->Radius;

	// If the squared distance is less than or equal to the squared sum of the radii, the spheres are colliding
	if (DistanceSquared <= FMath::Square(RadiusSum))
	{
		// Calculate the actual distance between the centers
		float Distance = FMath::Sqrt(DistanceSquared);

		// Calculate the contact point as the midpoint between the centers
		Output.ContactPoint = (A->Center + B->Center) * 0.5f;

		// Compute the penetration depth (the amount by which the spheres overlap)
		Output.PenetrationDepth = RadiusSum - Distance;

		// Calculate the contact normal as the normalized direction from A to B
		if (Distance > KINDA_SMALL_NUMBER) // Avoid division by zero
		{
			Output.ContactNormal = (B->Center - A->Center).GetSafeNormal();
		}
		else
		{
			Output.ContactNormal = FVector::ZeroVector; // If the centers overlap perfectly
		}

		return true;
	}

	// No collision
	return false;
}

// Narrow-phase collision check for BoundingSphere vs OBB
bool UTetherCollisionDetectionHandler::Narrow_BoundingSphere_OBB(const FTetherShape_BoundingSphere* A, const FTetherShape_OrientedBoundingBox* B, FNarrowPhaseCollision& Output)
{
	// Implement a more precise narrow-phase check for BoundingSphere vs OBB
	if (Broad_BoundingSphere_OBB(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.ContactPoint = (A->Center + B->GetLocalSpaceCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Narrow-phase collision check for BoundingSphere vs Capsule
bool UTetherCollisionDetectionHandler::Narrow_BoundingSphere_Capsule(const FTetherShape_BoundingSphere* A, const FTetherShape_Capsule* B, FNarrowPhaseCollision& Output)
{
	// Implement a more precise narrow-phase check for BoundingSphere vs Capsule
	if (Broad_BoundingSphere_Capsule(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.ContactPoint = (A->Center + B->GetLocalSpaceCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Symmetric narrow-phase collision check for BoundingSphere vs Pipe
bool UTetherCollisionDetectionHandler::Narrow_BoundingSphere_Pipe(const FTetherShape_BoundingSphere* A,
	const FTetherShape_Pipe* B, FNarrowPhaseCollision& Output)
{
	return Narrow_Pipe_BoundingSphere(B, A, Output); // Symmetric to Pipe vs BoundingSphere
}

// Narrow-phase collision check for OBB vs AABB
bool UTetherCollisionDetectionHandler::Narrow_OBB_AABB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B, FNarrowPhaseCollision& Output)
{
    // Get the center and half-extents of the AABB
    FVector BCenter = B->GetLocalSpaceCenter();
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
    Output.ContactPoint = (ACenter + BCenter) * 0.5f; // Simplified contact point
    Output.PenetrationDepth = 0.0f; // Placeholder; refine based on actual overlap calculations

    return true;
}

// Narrow-phase collision check for OBB vs BoundingSphere
bool UTetherCollisionDetectionHandler::Narrow_OBB_BoundingSphere(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_BoundingSphere* B, FNarrowPhaseCollision& Output)
{
	// Implement a more precise narrow-phase check for OBB vs BoundingSphere
	return Narrow_BoundingSphere_OBB(B, A, Output); // Symmetric to BoundingSphere vs OBB
}

// Narrow-phase collision check for OBB vs OBB
bool UTetherCollisionDetectionHandler::Narrow_OBB_OBB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B, FNarrowPhaseCollision& Output)
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

    float MinPenetrationDepth = FLT_MAX; // Track the smallest overlap
    FVector BestAxis = FVector::ZeroVector; // Track the best axis for penetration

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

        // Calculate the overlap (penetration depth) on this axis
        float PenetrationDepth = (AProjection + BProjection) - Distance;
        if (PenetrationDepth < MinPenetrationDepth)
        {
            MinPenetrationDepth = PenetrationDepth;
            BestAxis = NormalizedAxis;
        }
    }

    // If all axes overlap, calculate the contact point and penetration depth
    Output.ContactPoint = (ACenter + BCenter) * 0.5f; // Simplified contact point
    Output.PenetrationDepth = MinPenetrationDepth; // Set the minimum penetration depth
    Output.ContactNormal = BestAxis; // Set the normal of the deepest penetration axis

    return true;
}

// Narrow-phase collision check for OBB vs Capsule
bool UTetherCollisionDetectionHandler::Narrow_OBB_Capsule(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Capsule* B, FNarrowPhaseCollision& Output)
{
	// Implement a more precise narrow-phase check for OBB vs Capsule
	if (Broad_OBB_Capsule(A, B))
	{
		// Placeholder: Assume collision at center points
		Output.ContactPoint = (A->GetLocalSpaceCenter() + B->GetLocalSpaceCenter()) * 0.5f;
		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
		return true;
	}
	return false;
}

// Symmetric narrow-phase collision check for OBB vs Pipe
bool UTetherCollisionDetectionHandler::Narrow_OBB_Pipe(const FTetherShape_OrientedBoundingBox* A,
	const FTetherShape_Pipe* B, FNarrowPhaseCollision& Output)
{
	return Narrow_Pipe_OBB(B, A, Output); // Symmetric to Pipe vs OBB
}

// Narrow-phase collision check for Capsule vs AABB
bool UTetherCollisionDetectionHandler::Narrow_Capsule_AABB(const FTetherShape_Capsule* A, const FTetherShape_AxisAlignedBoundingBox* B, FNarrowPhaseCollision& Output)
{
	return Narrow_AABB_Capsule(B, A, Output); // Symmetric to AABB vs Capsule
}

// Narrow-phase collision check for Capsule vs BoundingSphere
bool UTetherCollisionDetectionHandler::Narrow_Capsule_BoundingSphere(const FTetherShape_Capsule* A, const FTetherShape_BoundingSphere* B, FNarrowPhaseCollision& Output)
{
	return Narrow_BoundingSphere_Capsule(B, A, Output); // Symmetric to BoundingSphere vs Capsule
}

// Narrow-phase collision check for Capsule vs OBB
bool UTetherCollisionDetectionHandler::Narrow_Capsule_OBB(const FTetherShape_Capsule* A, const FTetherShape_OrientedBoundingBox* B, FNarrowPhaseCollision& Output)
{
	return Narrow_OBB_Capsule(B, A, Output); // Symmetric to OBB vs Capsule
}

bool UTetherCollisionDetectionHandler::Narrow_Capsule_Capsule(const FTetherShape_Capsule* A, const FTetherShape_Capsule* B, FNarrowPhaseCollision& Output)
{
    // Calculate the top and bottom points of Capsule A (including hemispheres)
    FVector A_Top = A->Center + A->Rotation.RotateVector(FVector::UpVector) * (A->HalfHeight - A->Radius);  // The half height goes to the extent, not the center of the hemisphere!
    FVector A_Bottom = A->Center - A->Rotation.RotateVector(FVector::UpVector) * (A->HalfHeight - A->Radius);

    // Calculate the top and bottom points of Capsule B (including hemispheres)
    FVector B_Top = B->Center + B->Rotation.RotateVector(FVector::UpVector) * (B->HalfHeight - B->Radius);
    FVector B_Bottom = B->Center - B->Rotation.RotateVector(FVector::UpVector) * (B->HalfHeight - B->Radius);

    // Find the closest points between the line segments representing the capsules
    FVector ClosestPointA, ClosestPointB;
    FMath::SegmentDistToSegmentSafe(A_Bottom, A_Top, B_Bottom, B_Top, ClosestPointA, ClosestPointB);

    // Compute the distance between the closest points
    float DistanceSquared = FVector::DistSquared(ClosestPointA, ClosestPointB);
    float CombinedRadii = A->Radius + B->Radius;

    // Check if the capsules overlap considering the hemispheres
    if (DistanceSquared <= FMath::Square(CombinedRadii))
    {
    	float Distance = FMath::Sqrt(DistanceSquared);

    	// Calculate the contact point as the midpoint between the closest points
    	Output.ContactPoint = (ClosestPointA + ClosestPointB) * 0.5f;

    	// Compute the penetration depth (the amount by which the capsules overlap)
    	Output.PenetrationDepth = CombinedRadii - Distance;
    	
    	// Calculate the contact normal as the normalized vector between the closest points
    	if (Distance > KINDA_SMALL_NUMBER)  // Avoid division by zero
    	{
    		Output.ContactNormal = (ClosestPointB - ClosestPointA).GetSafeNormal();
    	}
    	else
    	{
    		Output.ContactNormal = FVector::ZeroVector; // If the capsules are perfectly overlapping
    	}
    	
        return true;
    }

    return false;
}

bool UTetherCollisionDetectionHandler::Narrow_Capsule_Pipe(const FTetherShape_Capsule* A, const FTetherShape_Pipe* B,
	FNarrowPhaseCollision& Output)
{
	// @todo
	return false;
	// // Calculate the top and bottom points of the capsule (including hemispheres)
	// FVector CapsuleTop = A->Center + A->Rotation.RotateVector(FVector::UpVector) * A->HalfHeight;
	// FVector CapsuleBottom = A->Center - A->Rotation.RotateVector(FVector::UpVector) * A->HalfHeight;
	//
	// // Find the closest point on the pipe to the capsule's line segment
	// float ClosestDistanceSquared = FLT_MAX;
	// FVector ClosestPointOnPipe;
	//
	// // Sample points along the arc of the pipe to find the closest one
	// constexpr int32 NumSamples = 36; // Adjust this for more precision
	// const float AngleStep = FMath::DegreesToRadians(B->ArcAngle) / NumSamples;
	//
	// for (int32 i = 0; i <= NumSamples; ++i)
	// {
	// 	float Angle = i * AngleStep;
	//
	// 	// Calculate a point on the outer surface of the pipe
	// 	FVector PointOnArcOuter = B->Center + B->Rotation.RotateVector(FVector(FMath::Cos(Angle) * B->OuterDimensions.X, FMath::Sin(Angle) * B->OuterDimensions.Y, 0.0f));
	//
	// 	// Offset by the thickness in the Z direction to represent the 3D thickness
	// 	PointOnArcOuter.Z += B->OuterDimensions.Z * 0.5f;
	//
	// 	// Find the closest point on the capsule's line segment to this point on the pipe's arc
	// 	FVector ClosestPointOnCapsule;
	// 	FMath::SegmentDistToSegmentSafe(CapsuleBottom, CapsuleTop, PointOnArcOuter, PointOnArcOuter, ClosestPointOnCapsule, PointOnArcOuter);
	//
	// 	float DistanceSquared = FVector::DistSquared(ClosestPointOnCapsule, PointOnArcOuter);
	// 	if (DistanceSquared < ClosestDistanceSquared)
	// 	{
	// 		ClosestDistanceSquared = DistanceSquared;
	// 		ClosestPointOnPipe = PointOnArcOuter;
	// 	}
	// }
	//
	// // Determine if the capsule's closest point is within the pipe's thickness
	// if (ClosestDistanceSquared <= FMath::Square(A->Radius))
	// {
	// 	Output.ContactPoint = ClosestPointOnPipe;
	// 	Output.PenetrationDepth = A->Radius - FMath::Sqrt(ClosestDistanceSquared);
	// 	return true;
	// }
	//
	// return false;
}

bool UTetherCollisionDetectionHandler::Narrow_Pipe_AABB(const FTetherShape_Pipe* A,
	const FTetherShape_AxisAlignedBoundingBox* B, FNarrowPhaseCollision& Output)
{
	// @todo
	return false;
	// // 1. Transform the AABB and Pipe to world space
	// FVector PipeCenter = A->Center;
	// FVector PipeOuterDimensions = A->OuterDimensions;
	// float PipeArcAngle = A->ArcAngle;
	// FQuat PipeRotation = A->Rotation.Quaternion();
	//
	// FVector AABBMin = B->Min;
	// FVector AABBMax = B->Max;
	//
	// // 2. Approximate the pipe as a set of line segments and check for intersection with AABB
	//
	// int32 NumSegments = FMath::Max(12, FMath::RoundToInt(PipeArcAngle / 5.f)); // More segments for smoother arcs
	// float AngleStep = PipeArcAngle / NumSegments;
	//
	// for (int32 i = 0; i < NumSegments; ++i)
	// {
	// 	float AngleStart = FMath::DegreesToRadians(i * AngleStep);
	// 	float AngleEnd = FMath::DegreesToRadians((i + 1) * AngleStep);
	//
	// 	// Start and end points of the pipe segment
	// 	FVector SegmentStart = PipeCenter + PipeRotation.RotateVector(FVector(PipeOuterDimensions.X * FMath::Cos(AngleStart), PipeOuterDimensions.Y * FMath::Sin(AngleStart), 0.0f));
	// 	FVector SegmentEnd = PipeCenter + PipeRotation.RotateVector(FVector(PipeOuterDimensions.X * FMath::Cos(AngleEnd), PipeOuterDimensions.Y * FMath::Sin(AngleEnd), 0.0f));
	//
	// 	// Check if this segment intersects with the AABB
	// 	if (FMath::LineBoxIntersection(FBox(AABBMin, AABBMax), SegmentStart, SegmentEnd, FVector::ZeroVector))
	// 	{
	// 		Output.ContactPoint = (SegmentStart + SegmentEnd) * 0.5f;
	// 		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
	// 		return true;
	// 	}
	// }
	//
	// return false;
}

bool UTetherCollisionDetectionHandler::Narrow_Pipe_BoundingSphere(const FTetherShape_Pipe* A,
	const FTetherShape_BoundingSphere* B, FNarrowPhaseCollision& Output)
{
	// @todo
	return false;
	// // Transform the pipe and sphere to world space
	// FVector PipeCenter = A->Center;
	// FVector PipeOuterDimensions = A->OuterDimensions;
	// float PipeArcAngle = A->ArcAngle;
	// FQuat PipeRotation = A->Rotation.Quaternion();
	//
	// FVector SphereCenter = B->Center;
	// float SphereRadius = B->Radius;
	//
	// // Approximate the pipe as a set of line segments
	// int32 NumSegments = FMath::Max(12, FMath::RoundToInt(PipeArcAngle / 5.f)); // More segments for smoother arcs
	// float AngleStep = PipeArcAngle / NumSegments;
	//
	// for (int32 i = 0; i < NumSegments; ++i)
	// {
	// 	float AngleStart = FMath::DegreesToRadians(i * AngleStep);
	// 	float AngleEnd = FMath::DegreesToRadians((i + 1) * AngleStep);
	//
	// 	// Start and end points of the pipe segment
	// 	FVector SegmentStart = PipeCenter + PipeRotation.RotateVector(FVector(PipeOuterDimensions.X * FMath::Cos(AngleStart), PipeOuterDimensions.Y * FMath::Sin(AngleStart), 0.0f));
	// 	FVector SegmentEnd = PipeCenter + PipeRotation.RotateVector(FVector(PipeOuterDimensions.X * FMath::Cos(AngleEnd), PipeOuterDimensions.Y * FMath::Sin(AngleEnd), 0.0f));
	//
	// 	// Find the closest point on the segment to the sphere's center
	// 	FVector ClosestPoint = FMath::ClosestPointOnSegment(SphereCenter, SegmentStart, SegmentEnd);
	//
	// 	// Check if the distance from the closest point to the sphere's center is less than the sphere's radius
	// 	if (FVector::DistSquared(ClosestPoint, SphereCenter) <= FMath::Square(SphereRadius))
	// 	{
	// 		Output.ContactPoint = ClosestPoint;
	// 		Output.PenetrationDepth = SphereRadius - FVector::Dist(ClosestPoint, SphereCenter);
	// 		return true;
	// 	}
	// }
	//
	// return false;
}

bool UTetherCollisionDetectionHandler::Narrow_Pipe_OBB(const FTetherShape_Pipe* A,
	const FTetherShape_OrientedBoundingBox* B, FNarrowPhaseCollision& Output)
{
	// @todo
	return false;
	// // Transform the pipe and OBB to world space
	// FVector PipeCenter = A->Center;
	// FVector PipeOuterDimensions = A->OuterDimensions;
	// float PipeArcAngle = A->ArcAngle;
	// FQuat PipeRotation = A->Rotation.Quaternion();
	//
	// FVector OBBExtent = B->Extent;
	// FQuat OBBRotation = B->Rotation.Quaternion();
	// FVector OBBCenter = B->Center;
	//
	// // Approximate the pipe as a set of line segments
	// int32 NumSegments = FMath::Max(12, FMath::RoundToInt(PipeArcAngle / 5.f)); // More segments for smoother arcs
	// float AngleStep = PipeArcAngle / NumSegments;
	//
	// for (int32 i = 0; i < NumSegments; ++i)
	// {
	// 	float AngleStart = FMath::DegreesToRadians(i * AngleStep);
	// 	float AngleEnd = FMath::DegreesToRadians((i + 1) * AngleStep);
	//
	// 	// Start and end points of the pipe segment
	// 	FVector SegmentStart = PipeCenter + PipeRotation.RotateVector(FVector(PipeOuterDimensions.X * FMath::Cos(AngleStart), PipeOuterDimensions.Y * FMath::Sin(AngleStart), 0.0f));
	// 	FVector SegmentEnd = PipeCenter + PipeRotation.RotateVector(FVector(PipeOuterDimensions.X * FMath::Cos(AngleEnd), PipeOuterDimensions.Y * FMath::Sin(AngleEnd), 0.0f));
	//
	// 	// Project the segment onto the OBB's axes
	// 	if (FMath::LineBoxIntersection(FBox::BuildAABB(OBBCenter, OBBExtent), SegmentStart, SegmentEnd, OBBRotation.RotateVector(FVector::ZeroVector)))
	// 	{
	// 		Output.ContactPoint = (SegmentStart + SegmentEnd) * 0.5f;
	// 		Output.PenetrationDepth = 0.0f; // Placeholder for actual penetration depth calculation
	// 		return true;
	// 	}
	// }
	//
	// return false;
}

bool UTetherCollisionDetectionHandler::Narrow_Pipe_Capsule(const FTetherShape_Pipe* A, const FTetherShape_Capsule* B,
	FNarrowPhaseCollision& Output)
{
	// @todo
	return false;
	// // Transform the pipe and capsule to world space
	// FVector PipeCenter = A->Center;
	// FVector PipeOuterDimensions = A->OuterDimensions;
	// float PipeArcAngle = A->ArcAngle;
	// FQuat PipeRotation = A->Rotation.Quaternion();
	//
	// FVector CapsuleCenter = B->Center;
	// float CapsuleHalfHeight = B->HalfHeight;
	// float CapsuleRadius = B->Radius;
	// FQuat CapsuleRotation = B->Rotation.Quaternion();
	//
	// // Approximate the pipe as a set of line segments
	// int32 NumSegments = FMath::Max(12, FMath::RoundToInt(PipeArcAngle / 5.f)); // More segments for smoother arcs
	// float AngleStep = PipeArcAngle / NumSegments;
	//
	// // Calculate the top and bottom points of the capsule
	// FVector CapsuleUp = CapsuleRotation.RotateVector(FVector::UpVector) * CapsuleHalfHeight;
	// FVector CapsuleTop = CapsuleCenter + CapsuleUp;
	// FVector CapsuleBottom = CapsuleCenter - CapsuleUp;
	//
	// for (int32 i = 0; i < NumSegments; ++i)
	// {
	// 	float AngleStart = FMath::DegreesToRadians(i * AngleStep);
	// 	float AngleEnd = FMath::DegreesToRadians((i + 1) * AngleStep);
	//
	// 	// Start and end points of the pipe segment
	// 	FVector SegmentStart = PipeCenter + PipeRotation.RotateVector(FVector(PipeOuterDimensions.X * FMath::Cos(AngleStart), PipeOuterDimensions.Y * FMath::Sin(AngleStart), 0.0f));
	// 	FVector SegmentEnd = PipeCenter + PipeRotation.RotateVector(FVector(PipeOuterDimensions.X * FMath::Cos(AngleEnd), PipeOuterDimensions.Y * FMath::Sin(AngleEnd), 0.0f));
	//
	// 	// Find the closest points between the pipe segment and the capsule's segment
	// 	FVector ClosestPointOnCapsule, ClosestPointOnPipe;
	// 	FMath::SegmentDistToSegmentSafe(CapsuleBottom, CapsuleTop, SegmentStart, SegmentEnd, ClosestPointOnCapsule, ClosestPointOnPipe);
	//
	// 	// Check if the distance between the closest points is less than the combined radii
	// 	if (FVector::DistSquared(ClosestPointOnCapsule, ClosestPointOnPipe) <= FMath::Square(CapsuleRadius))
	// 	{
	// 		Output.ContactPoint = (ClosestPointOnCapsule + ClosestPointOnPipe) * 0.5f;
	// 		Output.PenetrationDepth = CapsuleRadius - FVector::Dist(ClosestPointOnCapsule, ClosestPointOnPipe);
	// 		return true;
	// 	}
	// }
	//
	// return false;
}

bool UTetherCollisionDetectionHandler::Narrow_Pipe_Pipe(const FTetherShape_Pipe* A, const FTetherShape_Pipe* B, FNarrowPhaseCollision& Output)
{
    // Transform both pipes to world space
    FVector PipeACenter = A->Center;
    float PipeAOuterRadius = A->OuterRadius;
    float PipeAInnerRadius = A->InnerRadius;
    float PipeAThickness = A->Thickness;
    float PipeAArcAngle = A->ArcAngle;
    FQuat PipeARotation = A->Rotation.Quaternion();

    FVector PipeBCenter = B->Center;
    float PipeBOuterRadius = B->OuterRadius;
    float PipeBInnerRadius = B->InnerRadius;
    float PipeBThickness = B->Thickness;
    float PipeBArcAngle = B->ArcAngle;
    FQuat PipeBRotation = B->Rotation.Quaternion();

    // Check if one pipe is entirely inside the other without overlapping
    float CenterDistance = FVector::Dist(PipeACenter, PipeBCenter);

    // If pipe A is inside pipe B
    if (CenterDistance < KINDA_SMALL_NUMBER && PipeAOuterRadius <= PipeBInnerRadius)
    {
        return false; // Pipe A is completely inside Pipe B, no collision
    }

    // If pipe B is inside pipe A
    if (CenterDistance < KINDA_SMALL_NUMBER && PipeBOuterRadius <= PipeAInnerRadius)
    {
        return false; // Pipe B is completely inside Pipe A, no collision
    }

    // Increase the number of segments for a finer approximation of the arc
    int32 NumSegmentsA = FMath::Max(24, FMath::RoundToInt(PipeAArcAngle / 2.f));  // More segments for better accuracy
    int32 NumSegmentsB = FMath::Max(24, FMath::RoundToInt(PipeBArcAngle / 2.f));

    float AngleStepA = PipeAArcAngle / NumSegmentsA;
    float AngleStepB = PipeBArcAngle / NumSegmentsB;

    float ClosestDistanceSquared = FLT_MAX;
    FVector ClosestPointA, ClosestPointB;

    // Iterate through points along the arcs of both pipes
    for (int32 i = 0; i < NumSegmentsA; ++i)
    {
        float AngleA = FMath::DegreesToRadians(i * AngleStepA);

        // Calculate points for both the outer and inner radii of Pipe A
        FVector OuterPointA = PipeACenter + PipeARotation.RotateVector(FVector(PipeAOuterRadius * FMath::Cos(AngleA), PipeAOuterRadius * FMath::Sin(AngleA), 0.0f));
        FVector InnerPointA = PipeACenter + PipeARotation.RotateVector(FVector(PipeAInnerRadius * FMath::Cos(AngleA), PipeAInnerRadius * FMath::Sin(AngleA), 0.0f));

        for (int32 j = 0; j < NumSegmentsB; ++j)
        {
            float AngleB = FMath::DegreesToRadians(j * AngleStepB);

            // Calculate points for both the outer and inner radii of Pipe B
            FVector OuterPointB = PipeBCenter + PipeBRotation.RotateVector(FVector(PipeBOuterRadius * FMath::Cos(AngleB), PipeBOuterRadius * FMath::Sin(AngleB), 0.0f));
            FVector InnerPointB = PipeBCenter + PipeBRotation.RotateVector(FVector(PipeBInnerRadius * FMath::Cos(AngleB), PipeBInnerRadius * FMath::Sin(AngleB), 0.0f));

            // Check the outer points of both pipes
            float DistanceSquaredOuter = FVector::DistSquared(OuterPointA, OuterPointB);

            if (DistanceSquaredOuter < ClosestDistanceSquared)
            {
                ClosestDistanceSquared = DistanceSquaredOuter;
                ClosestPointA = OuterPointA;
                ClosestPointB = OuterPointB;
            }

            // Check the inner points (if needed)
            if (PipeAInnerRadius > 0.0f && PipeBInnerRadius > 0.0f)
            {
                float DistanceSquaredInner = FVector::DistSquared(InnerPointA, InnerPointB);

                if (DistanceSquaredInner < ClosestDistanceSquared)
                {
                    ClosestDistanceSquared = DistanceSquaredInner;
                    ClosestPointA = InnerPointA;
                    ClosestPointB = InnerPointB;
                }
            }
        }
    }

    // Check if the closest points are within the combined thickness of the pipes
    float CombinedThickness = PipeAThickness + PipeBThickness;
    if (ClosestDistanceSquared <= FMath::Square(CombinedThickness))
    {
        float ClosestDistance = FMath::Sqrt(ClosestDistanceSquared);

        // Calculate the contact point and penetration depth
        Output.ContactPoint = (ClosestPointA + ClosestPointB) * 0.5f;
        Output.PenetrationDepth = CombinedThickness - ClosestDistance;

        // Calculate the contact normal (direction of penetration)
        if (ClosestDistance > KINDA_SMALL_NUMBER)
        {
            Output.ContactNormal = (ClosestPointB - ClosestPointA).GetSafeNormal();
        }
        else
        {
            Output.ContactNormal = FVector::ZeroVector; // If perfectly overlapping
        }

        return true;
    }

    return false;
}
