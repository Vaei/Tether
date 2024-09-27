// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShape_BoundingSphere.h"

#include "Shapes/TetherShapeCaster.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"
#include "System/TetherDrawing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_BoundingSphere)

FTetherShape_BoundingSphere::FTetherShape_BoundingSphere(const FVector& InCenter, float InRadius)
	: Center(InCenter)
	, Radius(InRadius)
{
	TetherShapeClass = UTetherShapeObject_BoundingSphere::StaticClass();

	// Caching initial local space data is required for duplication
	if (!IsWorldSpace())
	{
		LocalSpaceData = MakeShared<FTetherShape_BoundingSphere>(*this);
	}
}

void FTetherShape_BoundingSphere::ToLocalSpace_Implementation()
{
	if (!IsWorldSpace())
	{
		return;
	}
	
	if (ensure(LocalSpaceData.IsValid()))
	{
		*this = *StaticCastSharedPtr<FTetherShape_BoundingSphere>(LocalSpaceData);
	}
}

FTetherShape_AxisAlignedBoundingBox FTetherShape_BoundingSphere::GetBoundingBox() const
{
	// Calculate the min and max extents of the AABB
	FVector Min = Center - FVector(Radius);
	FVector Max = Center + FVector(Radius);

	return FTetherShape_AxisAlignedBoundingBox(Min, Max, IsWorldSpace(), AppliedWorldTransform);
}

FVector UTetherShapeObject_BoundingSphere::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	const auto* Sphere = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&Shape);
	return Sphere->Center;
}

void UTetherShapeObject_BoundingSphere::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	auto* Sphere = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&Shape);

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
	FVector TransformedCenter = WorldTransform.TransformPosition(Sphere->Center);

	// Scale the radius based on the scale of the world transform
	FVector Scale = WorldTransform.GetScale3D();
	float MaxScale = FMath::Max(Scale.X, FMath::Max(Scale.Y, Scale.Z));
	float TransformedRadius = Sphere->Radius * MaxScale;

	// Update the sphere with the transformed values
	Sphere->Center = TransformedCenter;
	Sphere->Radius = TransformedRadius;
}

void UTetherShapeObject_BoundingSphere::TransformToLocalSpace(FTetherShape& Shape) const
{
	if (!Shape.IsWorldSpace())
	{
		// Already there
		return;
	}
	
	auto* CastShape = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&Shape);
	CastShape->ToLocalSpace_Implementation();
}

FTetherShape_AxisAlignedBoundingBox UTetherShapeObject_BoundingSphere::GetBoundingBox(const FTetherShape& Shape) const
{
	const auto* BoundingSphere = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&Shape);
	return BoundingSphere->GetBoundingBox();
}

void UTetherShapeObject_BoundingSphere::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy,
	const UWorld* World, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	const auto* Sphere = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&Shape);
	UTetherDrawing::DrawSphere(World, Proxy, Sphere->Center, Sphere->Radius, 16, Color, bPersistentLines, LifeTime, Thickness);
#endif
}
