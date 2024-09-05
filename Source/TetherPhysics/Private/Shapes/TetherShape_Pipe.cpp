// Copyright (c) Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_Pipe.h"
#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShapeCaster.h"

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

void UTetherShapeObject_Pipe::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
    const auto* Pipe = FTetherShapeCaster::CastChecked<FTetherShape_Pipe>(&Shape);

    const FVector& Center = Pipe->Center;
    const FVector& OuterDimensions = Pipe->OuterDimensions;
    const float ArcAngle = Pipe->ArcAngle;
    const FRotator& Rotation = Pipe->Rotation;

    float OuterRadius = FMath::Max(OuterDimensions.X, OuterDimensions.Y) * 0.5f;
    float InnerRadius = FMath::Min(OuterDimensions.X, OuterDimensions.Y) * 0.5f;
    float PipeThickness = OuterDimensions.Z;

    FVector UpVector = Rotation.RotateVector(FVector::UpVector);
    FVector RightVector = Rotation.RotateVector(FVector::RightVector);
    FVector ForwardVector = Rotation.RotateVector(FVector::ForwardVector);

    int32 NumSegments = FMath::Max(1, FMath::RoundToInt(ArcAngle / 14.f));
    float AngleStep = ArcAngle / NumSegments;

    TArray<FVector> OuterBottomPoints, InnerBottomPoints, OuterTopPoints, InnerTopPoints;
    OuterBottomPoints.SetNum(NumSegments + 1);
    InnerBottomPoints.SetNum(NumSegments + 1);
    OuterTopPoints.SetNum(NumSegments + 1);
    InnerTopPoints.SetNum(NumSegments + 1);

    // Generate points along the arc for both inner and outer circles
    for (int32 i = 0; i <= NumSegments; ++i)
    {
        float AngleRad = FMath::DegreesToRadians(i * AngleStep);

        FVector OuterOffset = ForwardVector * (OuterRadius * FMath::Cos(AngleRad)) + RightVector * (OuterRadius * FMath::Sin(AngleRad));
        FVector InnerOffset = ForwardVector * (InnerRadius * FMath::Cos(AngleRad)) + RightVector * (InnerRadius * FMath::Sin(AngleRad));

        // Calculate points for the bottom and top layers
        OuterBottomPoints[i] = Center + OuterOffset - UpVector * (PipeThickness * 0.5f);
        InnerBottomPoints[i] = Center + InnerOffset - UpVector * (PipeThickness * 0.5f);
        OuterTopPoints[i] = Center + OuterOffset + UpVector * (PipeThickness * 0.5f);
        InnerTopPoints[i] = Center + InnerOffset + UpVector * (PipeThickness * 0.5f);

        // Draw the vertical lines connecting the top and bottom layers
        DrawDebugLine(World, OuterBottomPoints[i], OuterTopPoints[i], Color, bPersistentLines, LifeTime, Thickness);
        DrawDebugLine(World, InnerBottomPoints[i], InnerTopPoints[i], Color, bPersistentLines, LifeTime, Thickness);

        // Draw the horizontal lines connecting the outer and inner layers at the bottom and top
        if (i > 0)
        {
            DrawDebugLine(World, OuterBottomPoints[i], OuterBottomPoints[i - 1], Color, bPersistentLines, LifeTime, Thickness);
            DrawDebugLine(World, InnerBottomPoints[i], InnerBottomPoints[i - 1], Color, bPersistentLines, LifeTime, Thickness);
            DrawDebugLine(World, OuterTopPoints[i], OuterTopPoints[i - 1], Color, bPersistentLines, LifeTime, Thickness);
            DrawDebugLine(World, InnerTopPoints[i], InnerTopPoints[i - 1], Color, bPersistentLines, LifeTime, Thickness);

            // Draw the diagonal connecting lines between the outer and inner layers (caps)
            DrawDebugLine(World, OuterBottomPoints[i], InnerBottomPoints[i], Color, bPersistentLines, LifeTime, Thickness);
            DrawDebugLine(World, OuterTopPoints[i], InnerTopPoints[i], Color, bPersistentLines, LifeTime, Thickness);
        }
    }

    // Close the arc if the angle is less than 360 degrees
    if (ArcAngle < 360.f)
    {
        DrawDebugLine(World, OuterBottomPoints[0], InnerBottomPoints[0], Color, bPersistentLines, LifeTime, Thickness);
        DrawDebugLine(World, OuterTopPoints[0], InnerTopPoints[0], Color, bPersistentLines, LifeTime, Thickness);
        DrawDebugLine(World, OuterBottomPoints[NumSegments], InnerBottomPoints[NumSegments], Color, bPersistentLines, LifeTime, Thickness);
        DrawDebugLine(World, OuterTopPoints[NumSegments], InnerTopPoints[NumSegments], Color, bPersistentLines, LifeTime, Thickness);
    }
#endif
}
