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
struct TETHER_API FTetherShape_AxisAlignedBoundingBox : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_AxisAlignedBoundingBox()
		: Min(FVector::ZeroVector)
		, Max(FVector::ZeroVector)
	{}

	FTetherShape_AxisAlignedBoundingBox(const FVector& InMin, const FVector& InMax)
		: Min(InMin)
		, Max(InMax)
	{}

	/** Minimum corner of the AABB */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Min;

	/** Maximum corner of the AABB */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Max;
	
	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox; }
};

UCLASS()
class TETHER_API UTetherShape_AxisAlignedBoundingBox : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox;	}

	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
