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
