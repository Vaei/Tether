﻿// Copyright (c) Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_Pipe.h"

#include "Shapes/TetherShapeCaster.h"
#include "System/TetherDrawing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_Pipe)

FTetherShape_Pipe::FTetherShape_Pipe(const FVector& InCenter, const FVector& InOuterDimensions, float InArcAngle, const FRotator& InRotation)
	: Center(InCenter)
	, OuterDimensions(InOuterDimensions)
	, ArcAngle(InArcAngle)
	, Rotation(InRotation)
{
	TetherShapeClass = UTetherShapeObject_Pipe::StaticClass();
	
	// Caching initial local space data is required for duplication
	if (!IsWorldSpace())
	{
		LocalSpaceData = MakeShared<FTetherShape_Pipe>(*this);
	}
}

void FTetherShape_Pipe::ToLocalSpace_Implementation()
{
	if (!IsWorldSpace())
	{
		return;
	}
	
	if (ensure(LocalSpaceData.IsValid()))
	{
		*this = *StaticCastSharedPtr<FTetherShape_Pipe>(LocalSpaceData);
	}
}

FTetherShape_AxisAlignedBoundingBox FTetherShape_Pipe::GetBoundingBox() const
{
	// Calculate the bounding box based on the pipe's outer dimensions and rotation
	FVector Extents = OuterDimensions * 0.5f;
	FVector Min = Center - Extents;
	FVector Max = Center + Extents;

	return FTetherShape_AxisAlignedBoundingBox(Min, Max, IsWorldSpace(), WorldTransform);
}

FVector UTetherShapeObject_Pipe::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	const auto* Pipe = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(&Shape);
	return Pipe->Center;
}

void UTetherShapeObject_Pipe::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	auto* Pipe = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(&Shape);

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

	// Clone current state prior to conversion
	Pipe->LocalSpaceData = Pipe->Clone();

	// Transform the center to world space
	FVector TransformedCenter = WorldTransform.TransformPosition(Pipe->Center);

	// Apply scaling to the outer dimensions based on the scale of the world transform
	FVector Scale = WorldTransform.GetScale3D();
	FVector TransformedOuterDimensions = Pipe->OuterDimensions * Scale;

	// Apply the rotation
	FRotator TransformedRotation = WorldTransform.GetRotation().Rotator() + Pipe->Rotation;

	// Update the pipe with the transformed values
	Pipe->Center = TransformedCenter;
	Pipe->OuterDimensions = TransformedOuterDimensions;
	Pipe->Rotation = TransformedRotation;
}

void UTetherShapeObject_Pipe::TransformToLocalSpace(FTetherShape& Shape) const
{
	if (!Shape.IsWorldSpace())
	{
		// Already there
		return;
	}

	auto* CastShape = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(&Shape);
	CastShape->ToLocalSpace_Implementation();
}

FTetherShape_AxisAlignedBoundingBox UTetherShapeObject_Pipe::GetBoundingBox(const FTetherShape& Shape) const
{
	const auto* Pipe = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(&Shape);
	return Pipe->GetBoundingBox();
}

void UTetherShapeObject_Pipe::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy, UWorld* World,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
    const auto* Pipe = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(&Shape);
	UTetherDrawing::DrawPipe(World, Proxy, Pipe->Center, Pipe->OuterDimensions, Pipe->ArcAngle, Pipe->Rotation, Color, bPersistentLines, LifeTime, Thickness);
#endif
}
