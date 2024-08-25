// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#include "Shapes/TetherShape_Cone.h"
#include "Animation/AnimInstanceProxy.h"
#include "Shapes/TetherShapeCaster.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape_Cone)

FTetherShape_Cone::FTetherShape_Cone()
	: BaseCenter(FVector::ZeroVector)
	, Height(20.f)
	, BaseRadius(10.f)
	, Rotation(FRotator::ZeroRotator)
{
	TetherShapeClass = UTetherShapeObject_Cone::StaticClass();
}

FTetherShape_Cone::FTetherShape_Cone(const FVector& InBaseCenter, float InHeight, float InBaseRadius, const FRotator& InRotation)
	: BaseCenter(InBaseCenter)
	, Height(InHeight)
	, BaseRadius(InBaseRadius)
	, Rotation(InRotation)
{
	TetherShapeClass = UTetherShapeObject_Cone::StaticClass();
}

FTetherShape_AxisAlignedBoundingBox FTetherShape_Cone::GetBoundingBox() const
{
	FTransform Transform = IsWorldSpace() ? WorldTransform : FTransform::Identity;
	FVector Tip = Transform.TransformPosition(BaseCenter + Rotation.RotateVector(FVector(0, 0, Height)));

	// Get the points that define the cone's bounding box
	TArray<FVector> Points;
	Points.Add(Transform.TransformPosition(BaseCenter + Rotation.RotateVector(FVector(BaseRadius, BaseRadius, 0))));
	Points.Add(Transform.TransformPosition(BaseCenter + Rotation.RotateVector(FVector(-BaseRadius, BaseRadius, 0))));
	Points.Add(Transform.TransformPosition(BaseCenter + Rotation.RotateVector(FVector(BaseRadius, -BaseRadius, 0))));
	Points.Add(Transform.TransformPosition(BaseCenter + Rotation.RotateVector(FVector(-BaseRadius, -BaseRadius, 0))));
	Points.Add(Tip);

	FVector Min = Points[0];
	FVector Max = Points[0];

	for (const FVector& Point : Points)
	{
		Min.X = FMath::Min(Min.X, Point.X);
		Min.Y = FMath::Min(Min.Y, Point.Y);
		Min.Z = FMath::Min(Min.Z, Point.Z);

		Max.X = FMath::Max(Max.X, Point.X);
		Max.Y = FMath::Max(Max.Y, Point.Y);
		Max.Z = FMath::Max(Max.Z, Point.Z);
	}

	return FTetherShape_AxisAlignedBoundingBox(Min, Max);
}


FVector UTetherShapeObject_Cone::GetLocalSpaceShapeCenter(const FTetherShape& Shape) const
{
	const FTetherShape_Cone* Cone = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&Shape);
	return Cone->BaseCenter;
}

void UTetherShapeObject_Cone::TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const
{
	FTetherShape_Cone* Cone = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&Shape);

	if (Shape.IsWorldSpace() && !Shape.GetWorldTransform().Equals(WorldTransform))
	{
		// Already in world space, but has a new transform. Convert it back first.
		TransformToLocalSpace(Shape);
	}

	// Transform the base center to world space
	FVector TransformedBaseCenter = WorldTransform.TransformPosition(Cone->BaseCenter);

	// Scale the height and base radius based on the scale of the world transform
	FVector Scale = WorldTransform.GetScale3D();
	float MaxScale = FMath::Max(Scale.X, FMath::Max(Scale.Y, Scale.Z));
	float TransformedHeight = Cone->Height * MaxScale;
	float TransformedBaseRadius = Cone->BaseRadius * MaxScale;

	// Apply the rotation
	FRotator TransformedRotation = WorldTransform.GetRotation().Rotator() + Cone->Rotation;

	// Update the cone with the transformed values
	Cone->BaseCenter = TransformedBaseCenter;
	Cone->Height = TransformedHeight;
	Cone->BaseRadius = TransformedBaseRadius;
	Cone->Rotation = TransformedRotation;
}

void UTetherShapeObject_Cone::TransformToLocalSpace(FTetherShape& Shape) const
{
	if (!Shape.IsWorldSpace())
	{
		// Already there
		return;
	}

	FTetherShape_Cone* Cone = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&Shape);

	// Inverse the world transform to get back to local space
	FTransform InverseTransform = Shape.GetWorldTransform().Inverse();

	// Transform the base center back to local space
	FVector LocalBaseCenter = InverseTransform.TransformPosition(Cone->BaseCenter);

	// Adjust the height and base radius back to local scale
	FVector Scale = InverseTransform.GetScale3D();
	float MaxScale = FMath::Max(Scale.X, FMath::Max(Scale.Y, Scale.Z));
	float LocalHeight = Cone->Height / MaxScale;
	float LocalBaseRadius = Cone->BaseRadius / MaxScale;

	// Apply the inverse rotation
	FRotator LocalRotation = Cone->Rotation - InverseTransform.GetRotation().Rotator();

	// Update the cone with the local values
	Cone->BaseCenter = LocalBaseCenter;
	Cone->Height = LocalHeight;
	Cone->BaseRadius = LocalBaseRadius;
	Cone->Rotation = LocalRotation;
}

void UTetherShapeObject_Cone::DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const
{
	const FTetherShape_Cone* Cone = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&Shape);

	// Draw cone
	if (AnimInstanceProxy)
	{
		AnimInstanceProxy->AnimDrawDebugCone(Cone->BaseCenter, Cone->Height, FVector::UpVector, FMath::Atan2(Cone->BaseRadius, Cone->Height), FMath::Atan2(Cone->BaseRadius, Cone->Height), 12, Color, bPersistentLines, LifeTime, SDPG_World, Thickness);
	}
	else if (World)
	{
		DrawDebugCone(World, Cone->BaseCenter, FVector::UpVector, Cone->Height, FMath::Atan2(Cone->BaseRadius, Cone->Height), FMath::Atan2(Cone->BaseRadius, Cone->Height), 12, Color, bPersistentLines, LifeTime, 0, Thickness);
	}
}
