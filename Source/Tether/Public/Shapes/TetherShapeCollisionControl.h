// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherShape.h"
#include "UObject/Object.h"
#include "TetherShapeCollisionControl.generated.h"

struct FTetherNarrowPhaseCollisionOutput;
struct FTetherShape_AxisAlignedBoundingBox;

/**
 * This class handles collision interactions between different shapes
 * If you want to add custom shapes, then you will need to subclass this and override
 * CheckBroadCollision() and CheckNarrowCollision() to handle the collision of your shapes.
 *
 * Your new class will need to be assigned to UTetherDeveloperSettings::ShapeCollisionControl,
 * via Project Settings
 */
UCLASS(Const, NotBlueprintType, NotBlueprintable)
class TETHER_API UTetherShapeCollisionControl : public UObject
{
	GENERATED_BODY()

public:
	/** Broad Collision is a simple overlap test that occurs prior to physics simulation */
	virtual bool CheckBroadCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB) const;
	
	/** Narrow Collision is a complex collision that occurs after the physics simulation */
	virtual bool CheckNarrowCollision(const FTetherShape& ShapeA, const FTetherShape& ShapeB, FTetherNarrowPhaseCollisionOutput& Output) const;

	static bool Broad_AABB_AABB(const FTetherShape_AxisAlignedBoundingBox* A, const FTetherShape_AxisAlignedBoundingBox* B);
	// static bool Broad_AABB_OBB(const FTetherShape_AxisAlignedBoundingBox& A, const FTetherShape_AxisAlignedBoundingBox& B);
	// static bool Broad_AABB_BoundingSphere(const FTetherShape_AxisAlignedBoundingBox& A, const FTetherShape_AxisAlignedBoundingBox& B);
	// static bool Broad_AABB_Capsule(const FTetherShape_AxisAlignedBoundingBox& A, const FTetherShape_AxisAlignedBoundingBox& B);
	// static bool Broad_AABB_Cone(const FTetherShape_AxisAlignedBoundingBox& A, const FTetherShape_AxisAlignedBoundingBox& B);
};
