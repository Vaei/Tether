// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_OrientedBoundingBox.h"
#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShapeCaster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_OrientedBoundingBox)

FTetherShape_OrientedBoundingBox::FTetherShape_OrientedBoundingBox()
	: Center(FVector::ZeroVector)
	, Extent(FVector(50.f, 50.f, 50.f))
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
	FRotator TransformedRotation = WorldTransform.GetRotation().Rotator() + OBB->Rotation;

	// Update the OBB with the transformed values
	OBB->Center = TransformedCenter;
	OBB->Extent = TransformedExtent;
	OBB->Rotation = TransformedRotation;
}

void UTetherShapeObject_OrientedBoundingBox::TransformToLocalSpace(FTetherShape& Shape) const
{
	if (!Shape.IsWorldSpace())
	{
		// Already there
		return;
	}

	FTetherShape_OrientedBoundingBox* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);

	// Inverse the world transform to get back to local space
	FTransform InverseTransform = Shape.GetWorldTransform().Inverse();

	// Transform the center back to local space
	FVector LocalCenter = InverseTransform.TransformPosition(OBB->Center);

	// Adjust the extent back to local scale
	FVector LocalExtent = InverseTransform.GetScale3D().Reciprocal() * OBB->Extent;

	// Apply the inverse rotation
	FRotator LocalRotation = OBB->Rotation - InverseTransform.GetRotation().Rotator();

	// Update the OBB with the local values
	OBB->Center = LocalCenter;
	OBB->Extent = LocalExtent;
	OBB->Rotation = LocalRotation;
}

void UTetherShapeObject_OrientedBoundingBox::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
	const FTetherShape_OrientedBoundingBox* OBB = FTetherShapeCaster::CastChecked<FTetherShape_OrientedBoundingBox>(&Shape);
	
	// Get vertices of the OBB
	TArray<FVector> Vertices = OBB->GetVertices();

	// Draw edges using individual lines
	if (AnimInstanceProxy)
	{
		// Front face (connecting Vertices[0] -> Vertices[3])
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[0], Vertices[1], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[1], Vertices[3], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[3], Vertices[2], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[2], Vertices[0], Color, bPersistentLines, LifeTime, Thickness);

		// Back face (connecting Vertices[4] -> Vertices[7])
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[4], Vertices[5], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[5], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[7], Vertices[6], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[6], Vertices[4], Color, bPersistentLines, LifeTime, Thickness);

		// Connecting edges between front and back face
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[0], Vertices[4], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[1], Vertices[5], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[2], Vertices[6], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[3], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)  // We don't use DrawDebugBox because we want to test this too
	{
		// Front face (connecting Vertices[0] -> Vertices[3])
		DrawDebugLine(World, Vertices[0], Vertices[1], Color, bPersistentLines, LifeTime, Thickness);
		DrawDebugLine(World, Vertices[1], Vertices[3], Color, bPersistentLines, LifeTime, Thickness);
		DrawDebugLine(World, Vertices[3], Vertices[2], Color, bPersistentLines, LifeTime, Thickness);
		DrawDebugLine(World, Vertices[2], Vertices[0], Color, bPersistentLines, LifeTime, Thickness);

		// Back face (connecting Vertices[4] -> Vertices[7])
		DrawDebugLine(World, Vertices[4], Vertices[5], Color, bPersistentLines, LifeTime, Thickness);
		DrawDebugLine(World, Vertices[5], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
		DrawDebugLine(World, Vertices[7], Vertices[6], Color, bPersistentLines, LifeTime, Thickness);
		DrawDebugLine(World, Vertices[6], Vertices[4], Color, bPersistentLines, LifeTime, Thickness);

		// Connecting edges between front and back face
		DrawDebugLine(World, Vertices[0], Vertices[4], Color, bPersistentLines, LifeTime, Thickness);
		DrawDebugLine(World, Vertices[1], Vertices[5], Color, bPersistentLines, LifeTime, Thickness);
		DrawDebugLine(World, Vertices[2], Vertices[6], Color, bPersistentLines, LifeTime, Thickness);
		DrawDebugLine(World, Vertices[3], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
	}
}
