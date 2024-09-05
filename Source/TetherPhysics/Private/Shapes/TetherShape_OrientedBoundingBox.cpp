// Copyright (c) Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_OrientedBoundingBox.h"
#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShapeCaster.h"

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

	return FTetherShape_AxisAlignedBoundingBox(Min, Max, IsWorldSpace(), WorldTransform);
}

FVector UTetherShapeObject_OrientedBoundingBox::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	const auto* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);
	return OBB->Center;
}

void UTetherShapeObject_OrientedBoundingBox::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	auto* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);

	if (Shape.IsWorldSpace())
	{
		// Already in world space
		if (!Shape.GetWorldTransform().Equals(WorldTransform))
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

void UTetherShapeObject_OrientedBoundingBox::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	const auto* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);
	
	// Get vertices of the OBB
	TArray<FVector> Vertices = OBB->GetVertices();

	// Draw edges using individual lines
	for (int32 i = 0; i < 4; ++i)
	{
		int32 NextIndex = (i + 1) % 4;

		if (AnimInstanceProxy)
		{
			// Draw the front face
			AnimInstanceProxy->AnimDrawDebugLine(Vertices[i], Vertices[NextIndex], Color, bPersistentLines, LifeTime, Thickness);
			AnimInstanceProxy->AnimDrawDebugLine(Vertices[i + 4], Vertices[NextIndex + 4], Color, bPersistentLines, LifeTime, Thickness);

			// Draw the connecting edges between the front and back face
			AnimInstanceProxy->AnimDrawDebugLine(Vertices[i], Vertices[i + 4], Color, bPersistentLines, LifeTime, Thickness);
		}
		else if (World)
		{
			// Draw the front face
			DrawDebugLine(World, Vertices[i], Vertices[NextIndex], Color, bPersistentLines, LifeTime, 0, Thickness);
			DrawDebugLine(World, Vertices[i + 4], Vertices[NextIndex + 4], Color, bPersistentLines, LifeTime, 0, Thickness);

			// Draw the connecting edges between the front and back face
			DrawDebugLine(World, Vertices[i], Vertices[i + 4], Color, bPersistentLines, LifeTime, 0, Thickness);
		}
	}
#endif
}
