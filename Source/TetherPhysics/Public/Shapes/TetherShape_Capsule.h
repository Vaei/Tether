// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_AxisAlignedBoundingBox.h"
#include "TetherShape_Capsule.generated.h"

/**
 * Represents a Capsule shape in the Tether physics system.
 *
 * A Capsule is defined by a cylindrical body with hemispherical ends. It is commonly used in collision detection
 * due to its smooth, rounded shape, which is ideal for characters and other objects that need to slide or roll
 * against surfaces. The Capsule is specified by its center, half-height, radius, and rotation, making it
 * versatile for various applications in physics simulations.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape_Capsule : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_Capsule()
		: FTetherShape_Capsule(FVector::ZeroVector, 25.f, 10.f, FRotator::ZeroRotator)
	{}

	FTetherShape_Capsule(const FVector& InCenter, float InHalfHeight, float InRadius, const FRotator& InRotation);

	/** Creates a clone of the Capsule shape, preserving its specific type and data */
	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_Capsule>(*this); }

	/** Returns the gameplay tag associated with this shape type */
	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_Capsule; }

	void ToLocalSpace_Implementation();

	FTetherShape_AxisAlignedBoundingBox GetBoundingBox() const;

	/** Center of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Center;

	/** Half-height of the capsule (from the center to one of the hemispherical ends) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float HalfHeight;

	/** Radius of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float Radius;

	/** Rotation of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FRotator Rotation;
};

/**
 * Defines the behavior and operations for a Capsule shape in the Tether physics system.
 *
 * This class provides the necessary virtual functions for managing and manipulating Capsule shapes,
 * including transformations between local and world space, as well as debugging visualizations.
 * The Capsule shape is well-suited for character collision and physics interactions that require
 * smooth, rounded surfaces.
 */
UCLASS()
class TETHERPHYSICS_API UTetherShapeObject_Capsule : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	/** Returns the gameplay tag that identifies the type of shape */
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_Capsule; }

	/** Returns the center of the shape in local space */
	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const override;

	/** Transforms the shape data from local space to world space */
	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;

	/** Transforms the shape data from world space back to local space */
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	/** Gets the shape as a bounding box */
	virtual FTetherShape_AxisAlignedBoundingBox GetBoundingBox(const FTetherShape& Shape) const override;

	/** Draws the shape for debugging purposes */
	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy, const UWorld* World,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
