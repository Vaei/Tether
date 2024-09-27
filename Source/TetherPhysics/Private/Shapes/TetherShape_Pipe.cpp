// Copyright (c) Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_Pipe.h"

#include "Shapes/TetherShapeCaster.h"
#include "System/TetherDrawing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_Pipe)

FTetherShape_Pipe::FTetherShape_Pipe(const FVector& InCenter, float InOuterRadius, float InInnerRadius,
	float InThickness, float InArcAngle, const FRotator& InRotation)
	: Center(InCenter)
	, Rotation(InRotation)
	, OuterRadius(InOuterRadius)
	, InnerRadius(InInnerRadius)
	, Thickness(InThickness)
	, ArcAngle(InArcAngle)
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
	// Calculate the extents based on the pipe's outer radius and thickness
	FVector Extents { OuterRadius, OuterRadius, Thickness * 0.5f };
    
	// Apply rotation to calculate the bounding box in world space
	FVector Min = Center - Extents;
	FVector Max = Center + Extents;

	return FTetherShape_AxisAlignedBoundingBox(Min, Max, IsWorldSpace(), AppliedWorldTransform);
}

FVector UTetherShapeObject_Pipe::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	if (Shape.IsWorldSpace())
	{
		if (ensureAlways(Shape.LocalSpaceData.IsValid()))
		{
			const auto* LocalPipe = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(Shape.LocalSpaceData.Get());
			return LocalPipe->Center;
		}
	}
	else
	{
		const auto* Pipe = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(&Shape);
		return Pipe->Center;
	}
	return FVector::ZeroVector;
}

void UTetherShapeObject_Pipe::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	auto* Pipe = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(&Shape);

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

	// Clone current state prior to conversion
	Pipe->LocalSpaceData = Pipe->Clone();

	// Transform the center to world space
	FVector TransformedCenter = WorldTransform.TransformPosition(Pipe->Center);

	// Apply scaling to the pipe's radii and thickness based on the scale of the world transform
	FVector Scale = WorldTransform.GetScale3D();
	float TransformedOuterRadius = Pipe->OuterRadius * Scale.X;  // Assuming uniform scale for the radius
	float TransformedInnerRadius = Pipe->InnerRadius * Scale.X;  // Same scaling for inner radius
	float TransformedThickness = Pipe->Thickness * Scale.Z;      // Thickness along Z-axis

	// Apply the rotation
	FRotator TransformedRotation = WorldTransform.GetRotation().Rotator() + Pipe->Rotation;

	// Update the pipe with the transformed values
	Pipe->Center = TransformedCenter;
	Pipe->OuterRadius = TransformedOuterRadius;
	Pipe->InnerRadius = TransformedInnerRadius;
	Pipe->Thickness = TransformedThickness;
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

void UTetherShapeObject_Pipe::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy, const UWorld* World,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
    const auto* Pipe = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(&Shape);
	UTetherDrawing::DrawPipe(World, Proxy, Pipe->Center, Pipe->OuterRadius, Pipe->InnerRadius, Pipe->Thickness, Pipe->ArcAngle, Pipe->Rotation, Color, bPersistentLines, LifeTime, Thickness);
#endif
}
