// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_AxisAlignedBoundingBox.h"
#include "TetherShape_Pipe.generated.h"

/**
 * Represents a Pipe shape in the Tether physics system.
 *
 * A Pipe is defined by its center, outer dimensions (width, height, and thickness), arc angle, and rotation.
 * The Pipe can be non-uniformly scaled and can represent partial arcs, making it versatile for various collision detection scenarios.
 * The Pipe's thickness is a 3D value, allowing it to have distinct width, height, and depth.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape_Pipe : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_Pipe()
		: FTetherShape_Pipe(FVector::ZeroVector, 20.f, 10.f, 10.f, 360.f, FRotator::ZeroRotator)
	{}

	FTetherShape_Pipe(const FVector& InCenter, float InOuterRadius, float InInnerRadius, float InThickness, float InArcAngle, const FRotator& InRotation);

	/** Creates a clone of the Pipe shape, preserving its specific type and data */
	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_Pipe>(*this); }

	/** Returns the gameplay tag associated with this shape type */
	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_Pipe; }

	void ToLocalSpace_Implementation();
	
	/** Calculates the axis-aligned bounding box that encapsulates the pipe */
	FTetherShape_AxisAlignedBoundingBox GetBoundingBox() const;
	
	/** Center of the pipe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Center;

	/** Rotation of the pipe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FRotator Rotation;
	
	/** Radius of the outermost section of the pipe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float OuterRadius;

	/** Radius of the innermost section of the pipe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float InnerRadius;

	/** Thickness of the pipe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float Thickness;

	/** Arc angle of the pipe in degrees (0 to 360), representing the portion of the pipe */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0", ClampMax="360", UIMax="360"))
	float ArcAngle;
};

/**
 * Defines the behavior and operations for a Pipe shape in the Tether physics system.
 *
 * This class provides the necessary virtual functions for managing and manipulating Pipe shapes,
 * including transformations between local and world space, as well as debugging visualizations.
 * The Pipe shape is ideal for representing objects with a hollow cylindrical body and adjustable arc,
 * such as helmets, rings, or other curved surfaces.
 */
UCLASS()
class TETHERPHYSICS_API UTetherShapeObject_Pipe : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	/** Returns the gameplay tag that identifies the type of shape */
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_Pipe; }

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
