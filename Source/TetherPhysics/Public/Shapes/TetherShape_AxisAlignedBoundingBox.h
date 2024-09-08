// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_AxisAlignedBoundingBox.generated.h"

/**
 * Represents an Axis-Aligned Bounding Box (AABB) in the Tether physics system.
 *
 * AABBs are simpler to compute compared to other bounding volumes, as they only require determining
 * the minimum and maximum extents of the object along the coordinate axes. Due to their axis-aligned nature,
 * overlap checks between two AABBs are computationally efficient, making them ideal for broad-phase
 * collision detection and other spatial queries.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape_AxisAlignedBoundingBox : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_AxisAlignedBoundingBox()
		: FTetherShape_AxisAlignedBoundingBox(FVector::OneVector * -10.f, FVector::OneVector * 10.f)
	{}

	FTetherShape_AxisAlignedBoundingBox(const FVector& InMin, const FVector& InMax)
		: FTetherShape_AxisAlignedBoundingBox(InMin, InMax, false, FTransform::Identity)
	{}

	FTetherShape_AxisAlignedBoundingBox(const FVector& InMin, const FVector& InMax, bool bInWorldSpace,
		const FTransform& InWorldTransform);

	/** Creates a clone of the AABB shape, preserving its specific type and data */
	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_AxisAlignedBoundingBox>(*this); }

	/** Returns the gameplay tag associated with this shape type */
	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox; }

	FVector GetBoxExtents() const {	return (Max - Min) * 0.5f; }
	
	void ToLocalSpace_Implementation();
	
	/** Minimum corner of the AABB, representing the smallest x, y, and z coordinates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Min;

	/** Maximum corner of the AABB, representing the largest x, y, and z coordinates */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Max;
};

/**
 * Defines the behavior and operations for an Axis-Aligned Bounding Box (AABB) in the Tether physics system.
 *
 * This class provides the necessary virtual functions for managing and manipulating AABB shapes,
 * including transformations between local and world space, as well as debugging visualizations.
 * The AABB shape is particularly useful for efficient collision detection due to its simple structure.
 */
UCLASS()
class TETHERPHYSICS_API UTetherShapeObject_AxisAlignedBoundingBox : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	/** Returns the gameplay tag that identifies the type of shape */
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox;	}

	/** Returns the center of the shape in local space */
	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const override;

	/** Transforms the shape data from local space to world space */
	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;

	/** Transforms the shape data from world space back to local space */
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	/** Gets the shape as a bounding box */
	virtual FTetherShape_AxisAlignedBoundingBox GetBoundingBox(const FTetherShape& Shape) const override;

	/** Draws the shape for debugging purposes */
	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy, UWorld* World,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
