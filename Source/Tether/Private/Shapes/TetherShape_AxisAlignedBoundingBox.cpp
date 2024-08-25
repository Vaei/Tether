// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShapeTypeCaster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_AxisAlignedBoundingBox)

void UTetherShape_AxisAlignedBoundingBox::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	FTetherShape_AxisAlignedBoundingBox* AABB = FTetherShapeTypeCaster::CastShapeChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);
	
	if (Shape.IsWorldSpace() && !Shape.GetWorldTransform().Equals(WorldTransform))
	{
		// Already in world space, but has a new transform. Convert it back first.
		TransformToLocalSpace(Shape);
	}
	
	FTetherShape TransformedShape;
	// Transform both the min and max points
	FVector TransformedMin = WorldTransform.TransformPosition(AABB->Min);
	FVector TransformedMax = WorldTransform.TransformPosition(AABB->Max);

	// Ensure the transformed min and max are correctly aligned
	AABB->Min = FVector(FMath::Min(TransformedMin.X, TransformedMax.X), 
				  FMath::Min(TransformedMin.Y, TransformedMax.Y), 
				  FMath::Min(TransformedMin.Z, TransformedMax.Z));

	AABB->Max = FVector(FMath::Max(TransformedMin.X, TransformedMax.X), 
				  FMath::Max(TransformedMin.Y, TransformedMax.Y), 
				  FMath::Max(TransformedMin.Z, TransformedMax.Z));

	Shape.ToWorldSpace(WorldTransform);
}

void UTetherShape_AxisAlignedBoundingBox::TransformToLocalSpace(FTetherShape& Shape) const
{
	if (!Shape.IsWorldSpace())
	{
		// Already there
		return;
	}
	
	FTetherShape_AxisAlignedBoundingBox* AABB = FTetherShapeTypeCaster::CastShapeChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);
	
	// Inverse the world transform to get back to local space
	FTransform InverseTransform = Shape.GetWorldTransform().Inverse();

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

	Shape.ToLocalSpace();
}

void UTetherShape_AxisAlignedBoundingBox::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
	const FTetherShape_AxisAlignedBoundingBox* AABB = FTetherShapeTypeCaster::CastShapeChecked<FTetherShape_AxisAlignedBoundingBox>(&Shape);

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
