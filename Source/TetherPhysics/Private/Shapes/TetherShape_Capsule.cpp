﻿// Copyright (c) Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_Capsule.h"

#include "Shapes/TetherShapeCaster.h"
#include "System/TetherDrawing.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_Capsule)

FTetherShape_Capsule::FTetherShape_Capsule(const FVector& InCenter, float InHalfHeight, float InRadius, const FRotator& InRotation)
	: Center(InCenter)
	, HalfHeight(InHalfHeight)
	, Radius(InRadius)
	, Rotation(InRotation)
{
	TetherShapeClass = UTetherShapeObject_Capsule::StaticClass();

	// Caching initial local space data is required for duplication
	if (!IsWorldSpace())
	{
		LocalSpaceData = MakeShared<FTetherShape_Capsule>(*this);
	}
}

void FTetherShape_Capsule::ToLocalSpace_Implementation()
{
	if (!IsWorldSpace())
	{
		return;
	}
	
	if (ensure(LocalSpaceData.IsValid()))
	{
		*this = *StaticCastSharedPtr<FTetherShape_Capsule>(LocalSpaceData);
	}
}

FTetherShape_AxisAlignedBoundingBox FTetherShape_Capsule::GetBoundingBox() const
{
	// Calculate the top and bottom points of the capsule
	FVector Up = Rotation.RotateVector(FVector(0, 0, HalfHeight));
	FVector Top = Center + Up + FVector(0, 0, Radius);
	FVector Bottom = Center - Up - FVector(0, 0, Radius);

	// Calculate min and max for the bounding box
	FVector Min = Top.ComponentMin(Bottom);
	FVector Max = Top.ComponentMax(Bottom);

	Min -= FVector(Radius, Radius, 0.0f);
	Max += FVector(Radius, Radius, 0.0f);

	return FTetherShape_AxisAlignedBoundingBox(Min, Max, IsWorldSpace(), AppliedWorldTransform);
}


FVector UTetherShapeObject_Capsule::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	if (Shape.IsWorldSpace())
	{
		if (ensureAlways(Shape.LocalSpaceData.IsValid()))
		{
			const auto* LocalCapsule = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(Shape.LocalSpaceData.Get());
			return LocalCapsule->Center;
		}
	}
	else
	{
		const auto* Capsule = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&Shape);
		return Capsule->Center;
	}
	return FVector::ZeroVector;
}

void UTetherShapeObject_Capsule::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	auto* Capsule = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&Shape);

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
	FVector TransformedCenter = WorldTransform.TransformPosition(Capsule->Center);

	// Apply scaling to the half-height and radius based on the scale of the world transform
	FVector Scale = WorldTransform.GetScale3D();

	// Use Z scale for HalfHeight if capsule is aligned along Z-axis
	float TransformedHalfHeight = Capsule->HalfHeight * Scale.Z;

	// Use average of X and Y scales for the radius (assuming cylindrical symmetry)
	float TransformedRadius = Capsule->Radius * FMath::Sqrt(Scale.X * Scale.Y);

	// Apply the rotation
	FRotator TransformedRotation = WorldTransform.GetRotation().Rotator() + Capsule->Rotation;

	// Update the capsule with the transformed values
	Capsule->Center = TransformedCenter;
	Capsule->HalfHeight = TransformedHalfHeight;
	Capsule->Radius = TransformedRadius;
	Capsule->Rotation = TransformedRotation;
}

void UTetherShapeObject_Capsule::TransformToLocalSpace(FTetherShape& Shape) const
{
	if (!Shape.IsWorldSpace())
	{
		// Already there
		return;
	}

	auto* CastShape = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&Shape);
	CastShape->ToLocalSpace_Implementation();
}

FTetherShape_AxisAlignedBoundingBox UTetherShapeObject_Capsule::GetBoundingBox(const FTetherShape& Shape) const
{
	const auto* Capsule = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&Shape);
	return Capsule->GetBoundingBox();
}

void UTetherShapeObject_Capsule::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy, const UWorld* World,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	const auto* Capsule = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&Shape);
	UTetherDrawing::DrawCapsule(World, Proxy, Capsule->Center, Capsule->HalfHeight, Capsule->Radius, Capsule->Rotation, Color, bPersistentLines, LifeTime, Thickness);
#endif
}
