// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShapeCaster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_AxisAlignedBoundingBox)

FTetherShape_AxisAlignedBoundingBox::FTetherShape_AxisAlignedBoundingBox()
	: Min(FVector::OneVector * -10.f)
    , Max(FVector::OneVector * 10.f)
{
	TetherShapeClass = UTetherShapeObject_AxisAlignedBoundingBox::StaticClass();
}

FTetherShape_AxisAlignedBoundingBox::FTetherShape_AxisAlignedBoundingBox(const FVector& InMin, const FVector& InMax)
	: Min(InMin)
	, Max(InMax)
{
	TetherShapeClass = UTetherShapeObject_AxisAlignedBoundingBox::StaticClass();
}

FVector UTetherShapeObject_AxisAlignedBoundingBox::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	const FTetherShape_AxisAlignedBoundingBox* AABB = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);
	return (AABB->Min + AABB->Max) * 0.5f;
}

void UTetherShapeObject_AxisAlignedBoundingBox::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	FTetherShape_AxisAlignedBoundingBox* AABB = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);

	if (Shape.IsWorldSpace() && !Shape.GetWorldTransform().Equals(WorldTransform))
	{
		// Already in world space, but has a new transform. Convert it back first.
		TransformToLocalSpace(Shape);
	}
	
	// Transform both the min and max points
	FTransform Transform = WorldTransform;
	Transform.SetRotation(FQuat::Identity);
	FVector TransformedMin = Transform.TransformPosition(AABB->Min);
	FVector TransformedMax = Transform.TransformPosition(AABB->Max);

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
	
	FTetherShape_AxisAlignedBoundingBox* AABB = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);
	
	// Inverse the world transform to get back to local space
	FTransform InverseTransform = Shape.GetWorldTransform().Inverse();
	InverseTransform.SetRotation(FQuat::Identity);

	// Transform both the min and max points back to local space
	FVector TransformedMin = InverseTransform.TransformPosition(AABB->Min);
	FVector TransformedMax = InverseTransform.TransformPosition(AABB->Max);

	// Ensure the transformed min and max are correctly aligned
	AABB->Min = FVector(FMath::Min(TransformedMin.X, TransformedMax.X), 
						FMath::Min(TransformedMin.Y, TransformedMax.Y), 
						FMath::Min(TransformedMin.Z, TransformedMax.Z));

	AABB->Max = FVector(FMath::Max(TransformedMin.X, TransformedMax.X), 
						FMath::Max(TransformedMin.Y, TransformedMax.Y), 
						FMath::Max(TransformedMin.Z, TransformedMax.Z));
}

void UTetherShapeObject_AxisAlignedBoundingBox::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy,
	UWorld* World, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	const FTetherShape_AxisAlignedBoundingBox* AABB = FTetherShapeCaster::CastChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);

	FVector Vertices[8];
	Vertices[0] = AABB->Min;
	Vertices[1] = FVector(AABB->Min.X, AABB->Min.Y, AABB->Max.Z);
	Vertices[2] = FVector(AABB->Min.X, AABB->Max.Y, AABB->Min.Z);
	Vertices[3] = FVector(AABB->Min.X, AABB->Max.Y, AABB->Max.Z);
	Vertices[4] = FVector(AABB->Max.X, AABB->Min.Y, AABB->Min.Z);
	Vertices[5] = FVector(AABB->Max.X, AABB->Min.Y, AABB->Max.Z);
	Vertices[6] = FVector(AABB->Max.X, AABB->Max.Y, AABB->Min.Z);
	Vertices[7] = AABB->Max;

	// Draw edges
	if (AnimInstanceProxy)
	{
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[0], Vertices[1], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[0], Vertices[2], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[0], Vertices[4], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[1], Vertices[3], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[1], Vertices[5], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[2], Vertices[3], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[2], Vertices[6], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[3], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[4], Vertices[5], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[4], Vertices[6], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[5], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[6], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)  // We don't use DrawDebugBox because we want to test this too
	{
		DrawDebugLine(World, Vertices[0], Vertices[1], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[0], Vertices[2], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[0], Vertices[4], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[1], Vertices[3], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[1], Vertices[5], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[2], Vertices[3], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[2], Vertices[6], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[3], Vertices[7], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[4], Vertices[5], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[4], Vertices[6], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[5], Vertices[7], Color, bPersistentLines, LifeTime, 0, Thickness);
		DrawDebugLine(World, Vertices[6], Vertices[7], Color, bPersistentLines, LifeTime, 0, Thickness);
	}
#endif
}
