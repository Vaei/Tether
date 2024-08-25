// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_AxisAlignedBoundingBox.generated.h"

/**
 * AABBs are simpler to compute, as they only require finding the minimum and maximum extents of the object along the
 * coordinate axes.
 *
 * Because they are axis-aligned, checking for overlaps between two AABBs is computationally cheap
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape_AxisAlignedBoundingBox : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_AxisAlignedBoundingBox();

	FTetherShape_AxisAlignedBoundingBox(const FVector& InMin, const FVector& InMax);

	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_AxisAlignedBoundingBox>(*this); }

	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox; }

	/**
	 * Minimum corner of the AABB
	 * Represents the coords of the corner of the box with the smallest x, y, and z values
	 * For a uniform box that is 10cm in size, this should be -5,-5,-5
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Min;

	/**
	 * Maximum corner of the AABB
	 * Represents the coordinates of the corner of the box with the largest x, y, and z values
	 * For a uniform box that is 10cm in size, this should be 5,5,5
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Max;
};

UCLASS()
class TETHERPHYSICS_API UTetherShapeObject_AxisAlignedBoundingBox : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox;	}

	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const override;
	
	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
