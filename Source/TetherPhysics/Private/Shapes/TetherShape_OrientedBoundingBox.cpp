// Copyright (c) Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_OrientedBoundingBox.h"
#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShapeCaster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_OrientedBoundingBox)

FTetherShape_OrientedBoundingBox::FTetherShape_OrientedBoundingBox()
	: Center(FVector::ZeroVector)
	, Extent(FVector::OneVector * 10.f)
	, Rotation(FRotator::ZeroRotator)
{
	TetherShapeClass = UTetherShapeObject_OrientedBoundingBox::StaticClass();
}

FTetherShape_OrientedBoundingBox::FTetherShape_OrientedBoundingBox(const FVector& InCenter, const FVector& InExtent, const FRotator& InRotation)
	: Center(InCenter)
	, Extent(InExtent)
	, Rotation(InRotation)
{
	TetherShapeClass = UTetherShapeObject_OrientedBoundingBox::StaticClass();
}

void FTetherShape_OrientedBoundingBox::ToLocalSpace_Implementation()
{
	if (ensure(LocalSpaceData.IsValid()))
	{
		*this = *StaticCastSharedPtr<FTetherShape_OrientedBoundingBox>(LocalSpaceData);
	}
}

FTetherShape_AxisAlignedBoundingBox FTetherShape_OrientedBoundingBox::GetBoundingBox() const
{
	FTransform Transform = IsWorldSpace() ? WorldTransform : FTransform::Identity;

	TArray<FVector> Vertices = GetVertices();

	FVector Min = Transform.TransformPosition(Vertices[0]);
	FVector Max = Transform.TransformPosition(Vertices[0]);

	for (const FVector& Vertex : Vertices)
	{
		FVector WorldVertex = Transform.TransformPosition(Vertex);
		Min.X = FMath::Min(Min.X, WorldVertex.X);
		Min.Y = FMath::Min(Min.Y, WorldVertex.Y);
		Min.Z = FMath::Min(Min.Z, WorldVertex.Z);

		Max.X = FMath::Max(Max.X, WorldVertex.X);
		Max.Y = FMath::Max(Max.Y, WorldVertex.Y);
		Max.Z = FMath::Max(Max.Z, WorldVertex.Z);
	}

	return FTetherShape_AxisAlignedBoundingBox(Min, Max);
}

FVector UTetherShapeObject_OrientedBoundingBox::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	const FTetherShape_OrientedBoundingBox* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);
	return OBB->Center;
}

void UTetherShapeObject_OrientedBoundingBox::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	FTetherShape_OrientedBoundingBox* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);

	if (Shape.IsWorldSpace() && !Shape.GetWorldTransform().Equals(WorldTransform))
	{
		// Already in world space, but has a new transform. Convert it back first.
		TransformToLocalSpace(Shape);
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

void UTetherShapeObject_OrientedBoundingBox::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	const FTetherShape_OrientedBoundingBox* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);
	
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