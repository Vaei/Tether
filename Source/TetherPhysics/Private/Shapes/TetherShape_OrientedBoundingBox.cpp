// Copyright (c) Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_OrientedBoundingBox.h"

#include "Shapes/TetherShapeCaster.h"
#include "System/TetherDrawing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_OrientedBoundingBox)

FTetherShape_OrientedBoundingBox::FTetherShape_OrientedBoundingBox(const FVector& InCenter, const FVector& InExtent, const FRotator& InRotation)
	: Center(InCenter)
	, Extent(InExtent)
	, Rotation(InRotation)
{
	TetherShapeClass = UTetherShapeObject_OrientedBoundingBox::StaticClass();

	// Caching initial local space data is required for duplication
	if (!IsWorldSpace())
	{
		LocalSpaceData = MakeShared<FTetherShape_OrientedBoundingBox>(*this);
	}
}

void FTetherShape_OrientedBoundingBox::ToLocalSpace_Implementation()
{
	if (!IsWorldSpace())
	{
		return;
	}
	
	if (ensure(LocalSpaceData.IsValid()))
	{
		*this = *StaticCastSharedPtr<FTetherShape_OrientedBoundingBox>(LocalSpaceData);
	}
}

FTetherShape_AxisAlignedBoundingBox FTetherShape_OrientedBoundingBox::GetBoundingBox() const
{
	TArray<FVector> Vertices = GetVertices();

	FVector Min = Vertices[0];
	FVector Max = Vertices[0];

	for (const FVector& Vertex : Vertices)
	{
		Min.X = FMath::Min(Min.X, Vertex.X);
		Min.Y = FMath::Min(Min.Y, Vertex.Y);
		Min.Z = FMath::Min(Min.Z, Vertex.Z);

		Max.X = FMath::Max(Max.X, Vertex.X);
		Max.Y = FMath::Max(Max.Y, Vertex.Y);
		Max.Z = FMath::Max(Max.Z, Vertex.Z);
	}

	return FTetherShape_AxisAlignedBoundingBox(Min, Max, IsWorldSpace(), AppliedWorldTransform);
}

FVector UTetherShapeObject_OrientedBoundingBox::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	if (Shape.IsWorldSpace())
	{
		if (ensureAlways(Shape.LocalSpaceData.IsValid()))
		{
			const auto* LocalOBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(Shape.LocalSpaceData.Get());
			return LocalOBB->Center;
		}
	}
	else
	{
		const auto* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);
		return OBB->Center;
	}
	return FVector::ZeroVector;
}

void UTetherShapeObject_OrientedBoundingBox::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	auto* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);

	if (Shape.IsWorldSpace())
	{
		// Already in world space
		if (!Shape.GetAppliedWorldTransform().Equals(WorldTransform))
		{
			// Transform has changed, revert to world first
			TransformToLocalSpace(Shape);
		}
		else
		{
			// No changes required
			return;
		}
	}

	if (!Shape.IsWorldSpace())
	{
		// Cache local space data
		Shape.LocalSpaceData = Shape.Clone();
	}

	// Transform the center to world space
	FVector TransformedCenter = WorldTransform.TransformPosition(OBB->Center);

	// Transform the extent based on the scale of the world transform
	FVector TransformedExtent = WorldTransform.GetScale3D() * OBB->Extent;

	// Apply the rotation
	FQuat TransformedRotation = WorldTransform.GetRotation() * OBB->Rotation.Quaternion();

	// Update the OBB with the transformed values
	OBB->Center = TransformedCenter;
	OBB->Extent = TransformedExtent;
	OBB->Rotation = TransformedRotation.Rotator();
}

void UTetherShapeObject_OrientedBoundingBox::TransformToLocalSpace(FTetherShape& Shape) const
{
	if (!Shape.IsWorldSpace())
	{
		// Already there
		return;
	}

	auto* CastShape = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);
	CastShape->ToLocalSpace_Implementation();
}

FTetherShape_AxisAlignedBoundingBox UTetherShapeObject_OrientedBoundingBox::GetBoundingBox(const FTetherShape& Shape) const
{
	const auto* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);
	return OBB->GetBoundingBox();
}

void UTetherShapeObject_OrientedBoundingBox::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy,
	const UWorld* World, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	const auto* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);
	UTetherDrawing::DrawBox(World, Proxy, OBB->Center, OBB->Extent, OBB->Rotation.Quaternion(), Color, bPersistentLines, LifeTime, Thickness);
#endif
}
