// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_AxisAlignedBoundingBox.h"
#include "TetherShape_OrientedBoundingBox.generated.h"

/**
 * Represents an Oriented Bounding Box (OBB) in the Tether physics system.
 *
 * Unlike Axis-Aligned Bounding Boxes, an Oriented Bounding Box can rotate and align with the object it encloses.
 * This makes OBBs more flexible and accurate for certain collision detection scenarios, especially when objects
 * are rotated. The OBB is defined by its center, extents, and rotation, and can be used in both broad-phase
 * and narrow-phase collision detection.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape_OrientedBoundingBox : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_OrientedBoundingBox()
		: FTetherShape_OrientedBoundingBox(FVector::ZeroVector, FVector::OneVector * 10.f, FRotator::ZeroRotator)
	{}

	FTetherShape_OrientedBoundingBox(const FVector& InCenter, const FVector& InExtent, const FRotator& InRotation);

	/** Creates a clone of the OBB shape, preserving its specific type and data */
	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_OrientedBoundingBox>(*this); }

	/** Returns the gameplay tag associated with this shape type */
	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_OrientedBoundingBox; }

	void ToLocalSpace_Implementation();
	
	/** Center of the OBB */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Center;

	/** Extents (half-sizes) of the OBB */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Extent;

	/** Rotation of the OBB */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FRotator Rotation;

	FTetherShape_AxisAlignedBoundingBox GetBoundingBox() const;

	/** Calculates and returns the vertices of the OBB */
	TArray<FVector> GetVertices() const
	{
		// Local axis vectors for the OBB (before rotation)
		FVector XAxis = Extent.X * FVector::ForwardVector;
		FVector YAxis = Extent.Y * FVector::RightVector;
		FVector ZAxis = Extent.Z * FVector::UpVector;

		// Apply rotation to these axes
		FQuat RotationQuat = Rotation.Quaternion();
		XAxis = RotationQuat.RotateVector(XAxis);
		YAxis = RotationQuat.RotateVector(YAxis);
		ZAxis = RotationQuat.RotateVector(ZAxis);

		// Calculate the eight vertices of the OBB
		TArray<FVector> Vertices;
		Vertices.SetNumUninitialized(8);

		Vertices[0] = Center - XAxis - YAxis - ZAxis;
		Vertices[1] = Center + XAxis - YAxis - ZAxis;
		Vertices[2] = Center + XAxis + YAxis - ZAxis;
		Vertices[3] = Center - XAxis + YAxis - ZAxis;

		Vertices[4] = Center - XAxis - YAxis + ZAxis;
		Vertices[5] = Center + XAxis - YAxis + ZAxis;
		Vertices[6] = Center + XAxis + YAxis + ZAxis;
		Vertices[7] = Center - XAxis + YAxis + ZAxis;

		return Vertices;
	}
};

/**
 * Defines the behavior and operations for an Oriented Bounding Box (OBB) in the Tether physics system.
 *
 * This class provides the necessary virtual functions for managing and manipulating OBB shapes,
 * including transformations between local and world space, as well as debugging visualizations.
 * OBBs offer more precise collision detection than AABBs when dealing with rotated objects, making them
 * useful for complex simulations.
 */
UCLASS()
class TETHERPHYSICS_API UTetherShapeObject_OrientedBoundingBox : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	/** Returns the gameplay tag that identifies the type of shape */
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_OrientedBoundingBox; }

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
