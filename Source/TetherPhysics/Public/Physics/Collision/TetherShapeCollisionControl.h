// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shapes/TetherShape.h"
#include "UObject/Object.h"
#include "TetherShapeCollisionControl.generated.h"

struct FTetherShape_Cone;
struct FTetherShape_Capsule;
struct FTetherShape_OrientedBoundingBox;
struct FTetherShape_BoundingSphere;
struct FTetherNarrowPhaseCollisionOutput;
struct FTetherShape_AxisAlignedBoundingBox;

/**
 * This class handles collision interactions between different shapes.
 *
 * This class repeats some behaviours, often its unnecessary due to symmetric collision
 * detection (order doesn't matter), however some narrow phase detections are potentially
 * asymmetric (order might matter), because the order can influence the complexity of
 * the calculations.
 * 
 * If you want to add custom shapes, then you will need to subclass this and override
 * CheckBroadCollision() and CheckNarrowCollision() to handle the collision of your shapes.
 *
 * Your new class will need to be assigned to UTetherDeveloperSettings::ShapeCollisionControl,
 * via Project Settings.
 */
UCLASS(Const, NotBlueprintType, NotBlueprintable)
class TETHERPHYSICS_API UTetherShapeCollisionControl : public UObject
{
	GENERATED_BODY()

public:
	static FVector ClampVector(const FVector& InVector, const FVector& Min, const FVector& Max);
	
	/** Broad Collision is a simple overlap test that occurs prior to physics simulation */
	virtual bool CheckBroadCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB) const;
	
	/** Narrow Collision is a complex collision that occurs after the physics simulation */
	virtual bool CheckNarrowCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB, FTetherNarrowPhaseCollisionOutput& Output) const;

	// Broad-phase collision checks
	static bool Broad_AABB_AABB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B);
	static bool Broad_AABB_BoundingSphere(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_BoundingSphere* B);
	static bool Broad_AABB_OBB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B);
	static bool Broad_AABB_Capsule(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Capsule* B);
	static bool Broad_AABB_Cone(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Cone* B);

	static bool Broad_BoundingSphere_AABB(const FTetherShape_BoundingSphere* A, const FTetherShape_AxisAlignedBoundingBox* B);
	static bool Broad_BoundingSphere_BoundingSphere(const FTetherShape_BoundingSphere* A, const FTetherShape_BoundingSphere* B);
	static bool Broad_BoundingSphere_OBB(const FTetherShape_BoundingSphere* A, const FTetherShape_OrientedBoundingBox* B);
	static bool Broad_BoundingSphere_Capsule(const FTetherShape_BoundingSphere* A, const FTetherShape_Capsule* B);
	static bool Broad_BoundingSphere_Cone(const FTetherShape_BoundingSphere* A, const FTetherShape_Cone* B);

	static bool Broad_OBB_AABB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B);
	static bool Broad_OBB_BoundingSphere(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_BoundingSphere* B);
	static bool Broad_OBB_OBB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B);
	static bool Broad_OBB_Capsule(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Capsule* B);
	static bool Broad_OBB_Cone(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Cone* B);

	static bool Broad_Capsule_AABB(const FTetherShape_Capsule* A, const FTetherShape_AxisAlignedBoundingBox* B);
	static bool Broad_Capsule_BoundingSphere(const FTetherShape_Capsule* A, const FTetherShape_BoundingSphere* B);
	static bool Broad_Capsule_OBB(const FTetherShape_Capsule* A, const FTetherShape_OrientedBoundingBox* B);
	static bool Broad_Capsule_Capsule(const FTetherShape_Capsule* A, const FTetherShape_Capsule* B);
	static bool Broad_Capsule_Cone(const FTetherShape_Capsule* A, const FTetherShape_Cone* B);

	static bool Broad_Cone_AABB(const FTetherShape_Cone* A, const FTetherShape_AxisAlignedBoundingBox* B);
	static bool Broad_Cone_BoundingSphere(const FTetherShape_Cone* A, const FTetherShape_BoundingSphere* B);
	static bool Broad_Cone_OBB(const FTetherShape_Cone* A, const FTetherShape_OrientedBoundingBox* B);
	static bool Broad_Cone_Capsule(const FTetherShape_Cone* A, const FTetherShape_Capsule* B);
	static bool Broad_Cone_Cone(const FTetherShape_Cone* A, const FTetherShape_Cone* B);

	// Narrow-phase collision checks
	static bool Narrow_AABB_AABB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_AABB_BoundingSphere(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_AABB_OBB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_AABB_Capsule(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_AABB_Cone(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output);

	static bool Narrow_BoundingSphere_AABB(const FTetherShape_BoundingSphere* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_BoundingSphere_BoundingSphere(const FTetherShape_BoundingSphere* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_BoundingSphere_OBB(const FTetherShape_BoundingSphere* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_BoundingSphere_Capsule(const FTetherShape_BoundingSphere* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_BoundingSphere_Cone(const FTetherShape_BoundingSphere* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output);

	static bool Narrow_OBB_AABB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_OBB_BoundingSphere(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_OBB_OBB(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_OBB_Capsule(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_OBB_Cone(const FTetherShape_OrientedBoundingBox* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output);

	static bool Narrow_Capsule_AABB(const FTetherShape_Capsule* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_Capsule_BoundingSphere(const FTetherShape_Capsule* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_Capsule_OBB(const FTetherShape_Capsule* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_Capsule_Capsule(const FTetherShape_Capsule* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_Capsule_Cone(const FTetherShape_Capsule* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output);

	static bool Narrow_Cone_AABB(const FTetherShape_Cone* A, const FTetherShape_AxisAlignedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_Cone_BoundingSphere(const FTetherShape_Cone* A, const FTetherShape_BoundingSphere* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_Cone_OBB(const FTetherShape_Cone* A, const FTetherShape_OrientedBoundingBox* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_Cone_Capsule(const FTetherShape_Cone* A, const FTetherShape_Capsule* B, FTetherNarrowPhaseCollisionOutput& Output);
	static bool Narrow_Cone_Cone(const FTetherShape_Cone* A, const FTetherShape_Cone* B, FTetherNarrowPhaseCollisionOutput& Output);
};
