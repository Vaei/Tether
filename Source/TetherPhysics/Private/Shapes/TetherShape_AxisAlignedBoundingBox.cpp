// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include "Shapes/TetherShapeCaster.h"
#include "System/TetherDrawing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_AxisAlignedBoundingBox)

FTetherShape_AxisAlignedBoundingBox::FTetherShape_AxisAlignedBoundingBox(const FVector& InMin, const FVector& InMax,
	bool bInWorldSpace, const FTransform& InWorldTransform)
	: Min(InMin)
	, Max(InMax)
{
	TetherShapeClass = UTetherShapeObject_AxisAlignedBoundingBox::StaticClass();

	// Caching initial local space data is required both for AABB being created to represent the bounds of other shapes,
	// and also for duplication
	if (!IsWorldSpace())
	{
		LocalSpaceData = MakeShared<FTetherShape_AxisAlignedBoundingBox>(*this);
	}

	// Bounding boxes are created from other shapes,
	// special handling is required when a world space shape wants a bounding box vs a local space shape
	if (bInWorldSpace)
	{
		// Now cache our world space data
		bWorldSpace = bInWorldSpace;
		AppliedWorldTransform = InWorldTransform;
	}
}

void FTetherShape_AxisAlignedBoundingBox::ToLocalSpace_Implementation()
{
	if (!IsWorldSpace())
	{
		return;
	}
	
	if (ensure(LocalSpaceData.IsValid()))
	{
		*this = *StaticCastSharedPtr<FTetherShape_AxisAlignedBoundingBox>(LocalSpaceData);
	}
}

FVector FTetherShape_AxisAlignedBoundingBox::ComputeCenter() const
{
	return (Min + Max) * 0.5f;
}

FVector UTetherShapeObject_AxisAlignedBoundingBox::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	if (Shape.IsWorldSpace())
	{
		if (ensureAlways(Shape.LocalSpaceData.IsValid()))
		{
			const auto* LocalAABB = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(Shape.LocalSpaceData.Get());
			return LocalAABB->ComputeCenter();
		}
	}
	else
	{
		const auto* AABB = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);
		return AABB->ComputeCenter();
	}
	return FVector::ZeroVector;
}

void UTetherShapeObject_AxisAlignedBoundingBox::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	auto* AABB = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);

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

	// Extract the scale from the WorldTransform
	FVector Scale = WorldTransform.GetScale3D();

	// Apply the scale to the Min and Max points of the AABB
	FVector ScaledMin = AABB->Min * Scale;
	FVector ScaledMax = AABB->Max * Scale;
	
	// Transform both the min and max points
	FTransform Transform = WorldTransform;
	Transform.SetRotation(FQuat::Identity);
	
	FVector TransformedMin = Transform.TransformPosition(ScaledMin);
	FVector TransformedMax = Transform.TransformPosition(ScaledMax);

	// Ensure the transformed min and max are correctly aligned
	AABB->Min = FVector(FMath::Min(TransformedMin.X, TransformedMax.X), 
						FMath::Min(TransformedMin.Y, TransformedMax.Y), 
						FMath::Min(TransformedMin.Z, TransformedMax.Z));

	AABB->Max = FVector(FMath::Max(TransformedMin.X, TransformedMax.X), 
						FMath::Max(TransformedMin.Y, TransformedMax.Y), 
						FMath::Max(TransformedMin.Z, TransformedMax.Z));
}

void UTetherShapeObject_AxisAlignedBoundingBox::TransformToLocalSpace(FTetherShape& Shape) const
{
	if (!Shape.IsWorldSpace())
	{
		// Already there
		return;
	}

	auto* CastShape = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);
	CastShape->ToLocalSpace_Implementation();
}

FTetherShape_AxisAlignedBoundingBox UTetherShapeObject_AxisAlignedBoundingBox::GetBoundingBox(const FTetherShape& Shape) const
{
	const auto* AABB = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);
	return *AABB;
}

void UTetherShapeObject_AxisAlignedBoundingBox::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy,
	const UWorld* World, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	const auto* AABB = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);

	// Calculate the center and extent of the AABB
	FVector Center = (AABB->Min + AABB->Max) * 0.5f;
	FVector Extent = (AABB->Max - AABB->Min) * 0.5f;

	// Draw the AABB using UTetherStatics::DrawBox
	UTetherDrawing::DrawBox(World, Proxy, Center, Extent, FQuat::Identity, Color, bPersistentLines, LifeTime, Thickness);
#endif
}
