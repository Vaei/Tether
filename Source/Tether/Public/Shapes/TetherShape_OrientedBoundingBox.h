// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_AxisAlignedBoundingBox.h"
#include "TetherShape_OrientedBoundingBox.generated.h"

USTRUCT(BlueprintType)
struct TETHER_API FTetherShape_OrientedBoundingBox : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_OrientedBoundingBox();

	FTetherShape_OrientedBoundingBox(const FVector& InCenter, const FVector& InExtent, const FRotator& InRotation);

	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_OrientedBoundingBox>(*this); }

	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_OrientedBoundingBox; }

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

	TArray<FVector> GetVertices() const
	{
		const FQuat QuatRotation = Rotation.Quaternion();
		TArray<FVector> Vertices;
		FVector Axes[3] =
		{
			QuatRotation.GetAxisX() * Extent.X,
			QuatRotation.GetAxisY() * Extent.Y,
			QuatRotation.GetAxisZ() * Extent.Z
		};

		for (int32 i = -1; i <= 1; i += 2)
		{
			for (int32 j = -1; j <= 1; j += 2)
			{
				for (int32 k = -1; k <= 1; k += 2)
				{
					Vertices.Add(Center + i * Axes[0] + j * Axes[1] + k * Axes[2]);
				}
			}
		}

		return Vertices;
	}
};

UCLASS()
class TETHER_API UTetherShapeObject_OrientedBoundingBox : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_OrientedBoundingBox; }

	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const override;

	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
