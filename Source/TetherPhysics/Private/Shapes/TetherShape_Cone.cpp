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

	// Calculate the direction vector (points from base to tip)
	FVector Direction = Rotation.RotateVector(FVector::UpVector);

	// Calculate the tip of the cone
	FVector Tip = Transform.TransformPosition(BaseCenter + Direction * Height);

	// Compute the base corners considering rotation
	FVector P1 = Transform.TransformPosition(BaseCenter + Rotation.RotateVector(FVector(BaseRadius, BaseRadius, 0)));
	FVector P2 = Transform.TransformPosition(BaseCenter + Rotation.RotateVector(FVector(-BaseRadius, BaseRadius, 0)));
	FVector P3 = Transform.TransformPosition(BaseCenter + Rotation.RotateVector(FVector(BaseRadius, -BaseRadius, 0)));
	FVector P4 = Transform.TransformPosition(BaseCenter + Rotation.RotateVector(FVector(-BaseRadius, -BaseRadius, 0)));

	// Start with Min and Max based on the base center itself
	FVector Min = BaseCenter;
	FVector Max = BaseCenter;

	// Include all transformed points in Min/Max calculations
	Min = Min.ComponentMin(P1).ComponentMin(P2).ComponentMin(P3).ComponentMin(P4).ComponentMin(Tip);
	Max = Max.ComponentMax(P1).ComponentMax(P2).ComponentMax(P3).ComponentMax(P4).ComponentMax(Tip);
	
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
		TransformToLocalSpace(Shape);
	}

	// Transform the base center to world space
	FVector TransformedBaseCenter = WorldTransform.TransformPosition(Cone->BaseCenter);

	// Apply uniform scaling
	FVector Scale = WorldTransform.GetScale3D();
	float UniformScale = FMath::Max(Scale.X, FMath::Max(Scale.Y, Scale.Z));
	float TransformedHeight = Cone->Height * UniformScale;
	float TransformedBaseRadius = Cone->BaseRadius * UniformScale;

	// Apply the rotation to the cone's rotation
	FQuat TransformedRotation = WorldTransform.GetRotation() * Cone->Rotation.Quaternion();

	// Update the cone with the transformed values
	Cone->BaseCenter = TransformedBaseCenter;
	Cone->Height = TransformedHeight;
	Cone->BaseRadius = TransformedBaseRadius;
	Cone->Rotation = TransformedRotation.Rotator();
}

void UTetherShapeObject_Cone::TransformToLocalSpace(FTetherShape& Shape) const
{
	if (!Shape.IsWorldSpace())
	{
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
#if ENABLE_DRAW_DEBUG
    const FTetherShape_Cone* Cone = FTetherShapeCaster::CastChecked<FTetherShape_Cone>(&Shape);

    // Calculate the direction vector (points from base to tip)
    FVector Direction = Cone->Rotation.RotateVector(FVector::UpVector); // Adjusted to point upwards

	// DEVNOTE: I do not trust DrawDebugCone functions to be accurate, largely due to my lack of understanding on how
	// to use them correctly, precision is important so drawing from scratch is beneficial here, but it allows me to
	// draw a cone with fewer segments and an end cap also, so it is ideal.

	auto OnDrawDebugLine = [&AnimInstanceProxy, &World](const FVector& LineStart, const FVector& LineEnd, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness)
	{
		if (AnimInstanceProxy)
		{
			AnimInstanceProxy->AnimDrawDebugLine(LineStart, LineEnd, Color, bPersistentLines, LifeTime, Thickness);
		}
		else if (World)
		{
			DrawDebugLine(World, LineStart, LineEnd, Color, bPersistentLines, LifeTime, 0, Thickness);
		}
	};
	
    // Calculate the tip of the cone
    FVector Tip = Cone->BaseCenter + Direction * Cone->Height;

    // Iterate over the segments to draw the circular base
    static constexpr int32 NumSegments = 8;
    float AngleStep = 2.0f * PI / NumSegments;
    FVector PreviousPoint;
	
	// Keep track of points for the X
	FVector PointA, PointB, PointC;

	for (int32 i = 0; i <= NumSegments; ++i)
    {
    	float Angle = i * AngleStep;
    	float X = Cone->BaseRadius * FMath::Cos(Angle);
    	float Y = Cone->BaseRadius * FMath::Sin(Angle);
    
    	FVector Point = Cone->BaseCenter + Cone->Rotation.RotateVector(FVector(X, Y, 0));
    
    	if (i > 0)
    	{
    		// Draw base circle segments
    		OnDrawDebugLine(PreviousPoint, Point, Color, bPersistentLines, LifeTime, Thickness);

    		// Draw lines from the tip to the current base point
    		OnDrawDebugLine(Tip, Point, Color, bPersistentLines, LifeTime, Thickness);
    	}
		
		// Store points for X
		if (i == NumSegments / 4)
		{
			PointA = Point;
		}
		else if (i == 3 * NumSegments / 4)
		{
			PointB = Point;
		}
		else if (i == NumSegments / 2)
		{
			PointC = Point;
		}

    	PreviousPoint = Point;
    }

    // Connect the last point to the first one to close the loop
    FVector FirstPoint = Cone->BaseCenter + Cone->Rotation.RotateVector(FVector(Cone->BaseRadius, 0, 0));
    OnDrawDebugLine(PreviousPoint, FirstPoint, Color, bPersistentLines, LifeTime, Thickness);
	
	// Draw the X across the base to cap it
	OnDrawDebugLine(PointA, PointB, Color, bPersistentLines, LifeTime, Thickness);
	OnDrawDebugLine(PointC, FirstPoint, Color, bPersistentLines, LifeTime, Thickness);
#endif
}