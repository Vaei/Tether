// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_Capsule.h"
#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShapeCaster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_Capsule)

FTetherShape_Capsule::FTetherShape_Capsule()
	: Center(FVector::ZeroVector)
	, HalfHeight(50.f)
	, Radius(25.f)
	, Rotation(FRotator::ZeroRotator)
{
	TetherShapeClass = UTetherShapeObject_Capsule::StaticClass();
}

FTetherShape_Capsule::FTetherShape_Capsule(const FVector& InCenter, float InHalfHeight, float InRadius, const FRotator& InRotation)
	: Center(InCenter)
	, HalfHeight(InHalfHeight)
	, Radius(InRadius)
	, Rotation(InRotation)
{
	TetherShapeClass = UTetherShapeObject_Capsule::StaticClass();
}

FTetherShape_AxisAlignedBoundingBox FTetherShape_Capsule::GetBoundingBox() const
{
	FTransform Transform = IsWorldSpace() ? WorldTransform : FTransform::Identity;

	// Calculate the top and bottom points of the capsule
	FVector Up = Rotation.RotateVector(FVector(0, 0, HalfHeight));
	FVector Top = Transform.TransformPosition(Center + Up + FVector(0, 0, Radius));
	FVector Bottom = Transform.TransformPosition(Center - Up - FVector(0, 0, Radius));

	// Calculate min and max for the bounding box
	FVector Min = Top.ComponentMin(Bottom);
	FVector Max = Top.ComponentMax(Bottom);

	Min -= FVector(Radius, Radius, 0.0f);
	Max += FVector(Radius, Radius, 0.0f);

	return FTetherShape_AxisAlignedBoundingBox(Min, Max);
}


FVector UTetherShapeObject_Capsule::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	const FTetherShape_Capsule* Capsule = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&Shape);
	return Capsule->Center;
}

void UTetherShapeObject_Capsule::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	FTetherShape_Capsule* Capsule = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&Shape);

	if (Shape.IsWorldSpace() && !Shape.GetWorldTransform().Equals(WorldTransform))
	{
		// Already in world space, but has a new transform. Convert it back first.
		TransformToLocalSpace(Shape);
	}

	// Transform the center to world space
	FVector TransformedCenter = WorldTransform.TransformPosition(Capsule->Center);

	// Scale the half-height and radius based on the scale of the world transform
	FVector Scale = WorldTransform.GetScale3D();
	float MaxScale = FMath::Max(Scale.X, FMath::Max(Scale.Y, Scale.Z));
	float TransformedHalfHeight = Capsule->HalfHeight * MaxScale;
	float TransformedRadius = Capsule->Radius * MaxScale;

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

	FTetherShape_Capsule* Capsule = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&Shape);

	// Inverse the world transform to get back to local space
	FTransform InverseTransform = Shape.GetWorldTransform().Inverse();

	// Transform the center back to local space
	FVector LocalCenter = InverseTransform.TransformPosition(Capsule->Center);

	// Adjust the half-height and radius back to local scale
	FVector Scale = InverseTransform.GetScale3D();
	float MaxScale = FMath::Max(Scale.X, FMath::Max(Scale.Y, Scale.Z));
	float LocalHalfHeight = Capsule->HalfHeight / MaxScale;
	float LocalRadius = Capsule->Radius / MaxScale;

	// Apply the inverse rotation
	FRotator LocalRotation = Capsule->Rotation - InverseTransform.GetRotation().Rotator();

	// Update the capsule with the local values
	Capsule->Center = LocalCenter;
	Capsule->HalfHeight = LocalHalfHeight;
	Capsule->Radius = LocalRadius;
	Capsule->Rotation = LocalRotation;
}

void UTetherShapeObject_Capsule::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
	const FTetherShape_Capsule* Capsule = FTetherShapeCaster::CastChecked<FTetherShape_Capsule>(&Shape);

	// Draw capsule
	if (AnimInstanceProxy)
	{
		AnimInstanceProxy->AnimDrawDebugCapsule(Capsule->Center, Capsule->HalfHeight, Capsule->Radius, Capsule->Rotation, Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)
	{
		DrawDebugCapsule(World, Capsule->Center, Capsule->HalfHeight, Capsule->Radius, Capsule->Rotation.Quaternion(), Color, bPersistentLines, LifeTime, 0, Thickness);
	}
}
