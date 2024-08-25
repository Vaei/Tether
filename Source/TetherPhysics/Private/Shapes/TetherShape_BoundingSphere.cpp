// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShape_BoundingSphere.h"

#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShapeCaster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_BoundingSphere)

FTetherShape_BoundingSphere::FTetherShape_BoundingSphere()
	: Center(FVector::ZeroVector)
    , Radius(10.f)
{
	TetherShapeClass = UTetherShapeObject_BoundingSphere::StaticClass();
}

FTetherShape_BoundingSphere::FTetherShape_BoundingSphere(const FVector& InCenter, float InRadius)
	: Center(InCenter)
	, Radius(InRadius)
{
	TetherShapeClass = UTetherShapeObject_BoundingSphere::StaticClass();
}

FVector UTetherShapeObject_BoundingSphere::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	const FTetherShape_BoundingSphere* Sphere = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&Shape);
	return Sphere->Center;
}

void UTetherShapeObject_BoundingSphere::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	FTetherShape_BoundingSphere* Sphere = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&Shape);

	if (Shape.IsWorldSpace() && !Shape.GetWorldTransform().Equals(WorldTransform))
	{
		// Already in world space, but has a new transform. Convert it back first.
		TransformToLocalSpace(Shape);
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
	
	FTetherShape_BoundingSphere* Sphere = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&Shape);

	// Inverse the world transform to get back to local space
	FTransform InverseTransform = Shape.GetWorldTransform().Inverse();

	// Transform the center back to local space
	FVector LocalCenter = InverseTransform.TransformPosition(Sphere->Center);

	// Adjust the radius back to local scale
	FVector Scale = InverseTransform.GetScale3D();
	float MaxScale = FMath::Max(Scale.X, FMath::Max(Scale.Y, Scale.Z));
	float LocalRadius = Sphere->Radius / MaxScale;

	// Update the sphere with the local values
	Sphere->Center = LocalCenter;
	Sphere->Radius = LocalRadius;
}

void UTetherShapeObject_BoundingSphere::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy,
	UWorld* World, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	const FTetherShape_BoundingSphere* Sphere = FTetherShapeCaster::CastChecked<FTetherShape_BoundingSphere>(&Shape);
	
	// Draw edges
	if (AnimInstanceProxy)
	{
		AnimInstanceProxy->AnimDrawDebugSphere(Sphere->Center, Sphere->Radius, 12, Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)
	{
		DrawDebugSphere(World, Sphere->Center, Sphere->Radius, 12, Color, bPersistentLines, LifeTime, 0, Thickness);
	}
#endif
}
