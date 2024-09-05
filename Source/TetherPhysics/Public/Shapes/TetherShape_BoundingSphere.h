// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_BoundingSphere.generated.h"

/**
 * Represents a Bounding Sphere in the Tether physics system.
 *
 * The Bounding Sphere is a simple and efficient bounding volume, defined by a center point and a radius.
 * It can be easily computed as the smallest sphere that encloses an object, with its radius determined
 * by the furthest point from the center. Collision detection with spheres is straightforward, often
 * involving basic distance checks, making this shape ideal for broad-phase collision detection.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape_BoundingSphere : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_BoundingSphere()
		: FTetherShape_BoundingSphere(FVector::ZeroVector, 10.f)
	{}

	FTetherShape_BoundingSphere(const FVector& InCenter, float InRadius);

	/** Creates a clone of the Bounding Sphere shape, preserving its specific type and data */
	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_BoundingSphere>(*this); }

	/** Returns the gameplay tag associated with this shape type */
	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_BoundingSphere; }

	void ToLocalSpace_Implementation();

	FTetherShape_AxisAlignedBoundingBox GetBoundingBox() const;
	
	/** Center of the sphere */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Center;

	/** Radius of the sphere */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float Radius;
};

/**
 * Defines the behavior and operations for a Bounding Sphere in the Tether physics system.
 *
 * This class provides the necessary virtual functions for managing and manipulating Bounding Sphere shapes,
 * including transformations between local and world space, as well as debugging visualizations.
 * The simplicity of the Bounding Sphere makes it ideal for fast collision checks and efficient spatial queries.
 */
UCLASS()
class TETHERPHYSICS_API UTetherShapeObject_BoundingSphere : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	/** Returns the gameplay tag that identifies the type of shape */
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_BoundingSphere;	}

	/** Returns the center of the shape in local space */
	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const override;

	/** Transforms the shape data from local space to world space */
	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;

	/** Transforms the shape data from world space back to local space */
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	/** Gets the shape as a bounding box */
	virtual FTetherShape_AxisAlignedBoundingBox GetBoundingBox(const FTetherShape& Shape) const override;

	/** Draws the shape for debugging purposes */
	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
