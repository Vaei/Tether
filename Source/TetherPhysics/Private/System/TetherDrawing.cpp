// Copyright (c) Jared Taylor. All Rights Reserved.


#include "System/TetherDrawing.h"

#include "Animation/AnimInstanceProxy.h"
#include "Engine/Canvas.h"
#include "System/TetherMessaging.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherDrawing)

static constexpr uint8 DepthPriority = 0;

void UTetherDrawing::DrawLine(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness)
{
#if ENABLE_DRAW_DEBUG
	if (Proxy)
	{
		Proxy->AnimDrawDebugLine(Start, End, Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)
	{
		DrawDebugLine(World, Start, End, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
#endif
}

void UTetherDrawing::DrawArrow(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End,
	const FColor& Color, float ArrowSize, bool bPersistentLines, float LifeTime, float Thickness)
{
#if ENABLE_DRAW_DEBUG
	if (Proxy)
	{
		Proxy->AnimDrawDebugDirectionalArrow(Start, End, ArrowSize, Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)
	{
		DrawDebugDirectionalArrow(World, Start, End, ArrowSize, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
#endif
}

void UTetherDrawing::DrawBox(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
	const FVector& Extent, const FQuat& Rotation, const FColor& Color, bool bPersistentLines, float LifeTime,
	float Thickness)
{
#if ENABLE_DRAW_DEBUG
    // Create a transform from the rotation
    FTransform Transform(Rotation);

    // Top face of the box
    FVector TopFrontRight = Transform.TransformPosition(FVector( Extent.X,  Extent.Y,  Extent.Z));
    FVector TopFrontLeft  = Transform.TransformPosition(FVector( Extent.X, -Extent.Y,  Extent.Z));
    FVector TopBackLeft   = Transform.TransformPosition(FVector(-Extent.X, -Extent.Y,  Extent.Z));
    FVector TopBackRight  = Transform.TransformPosition(FVector(-Extent.X,  Extent.Y,  Extent.Z));

    // Bottom face of the box
    FVector BottomFrontRight = Transform.TransformPosition(FVector( Extent.X,  Extent.Y, -Extent.Z));
    FVector BottomFrontLeft  = Transform.TransformPosition(FVector( Extent.X, -Extent.Y, -Extent.Z));
    FVector BottomBackLeft   = Transform.TransformPosition(FVector(-Extent.X, -Extent.Y, -Extent.Z));
    FVector BottomBackRight  = Transform.TransformPosition(FVector(-Extent.X,  Extent.Y, -Extent.Z));

    // Draw top face (4 lines)
    DrawLine(World, Proxy, Center + TopFrontRight, Center + TopFrontLeft, Color, bPersistentLines, LifeTime, Thickness);
    DrawLine(World, Proxy, Center + TopFrontLeft, Center + TopBackLeft, Color, bPersistentLines, LifeTime, Thickness);
    DrawLine(World, Proxy, Center + TopBackLeft, Center + TopBackRight, Color, bPersistentLines, LifeTime, Thickness);
    DrawLine(World, Proxy, Center + TopBackRight, Center + TopFrontRight, Color, bPersistentLines, LifeTime, Thickness);

    // Draw bottom face (4 lines)
    DrawLine(World, Proxy, Center + BottomFrontRight, Center + BottomFrontLeft, Color, bPersistentLines, LifeTime, Thickness);
    DrawLine(World, Proxy, Center + BottomFrontLeft, Center + BottomBackLeft, Color, bPersistentLines, LifeTime, Thickness);
    DrawLine(World, Proxy, Center + BottomBackLeft, Center + BottomBackRight, Color, bPersistentLines, LifeTime, Thickness);
    DrawLine(World, Proxy, Center + BottomBackRight, Center + BottomFrontRight, Color, bPersistentLines, LifeTime, Thickness);

    // Draw vertical edges connecting top and bottom faces (4 lines)
    DrawLine(World, Proxy, Center + TopFrontRight, Center + BottomFrontRight, Color, bPersistentLines, LifeTime, Thickness);
    DrawLine(World, Proxy, Center + TopFrontLeft, Center + BottomFrontLeft, Color, bPersistentLines, LifeTime, Thickness);
    DrawLine(World, Proxy, Center + TopBackLeft, Center + BottomBackLeft, Color, bPersistentLines, LifeTime, Thickness);
    DrawLine(World, Proxy, Center + TopBackRight, Center + BottomBackRight, Color, bPersistentLines, LifeTime, Thickness);
#endif
}

void UTetherDrawing::DrawSphere(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float Radius,
	int32 Segments, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness)
{
#if ENABLE_DRAW_DEBUG
	if (Proxy)
	{
		Proxy->AnimDrawDebugSphere(Center, Radius, Segments, Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)
	{
		DrawDebugSphere(World, Center, Radius, Segments, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
	}
#endif
}

void UTetherDrawing::DrawCapsule(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
	float HalfHeight, float Radius, const FRotator& Rotation, const FColor& Color, bool bPersistentLines, float LifeTime,
	float Thickness)
{
#if ENABLE_DRAW_DEBUG
	if (Proxy)
	{
		// @todo - Do we need to do the fill shape below here too?
		Proxy->AnimDrawDebugCapsule(Center, HalfHeight, Radius, Rotation, Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)
	{
		DrawDebugCapsule(World, Center, HalfHeight, Radius, Rotation.Quaternion(), Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		
		// Draw lines to fill the shape, for some reason DrawDebugCapsule is missing two of the vertical lines
		constexpr int32 NumSegments = 4; // Number of segments to approximate the capsule shape
		constexpr float AngleStep = 360.0f / NumSegments;

		const FVector UpVector = Rotation.RotateVector(FVector::UpVector);
		FVector TopCenter = Center + UpVector * HalfHeight + UpVector * -Radius;
		FVector BottomCenter = Center - UpVector * HalfHeight + UpVector * Radius;

		for (int32 i = 0; i < NumSegments; i++)
		{
			float CurrentAngle = FMath::DegreesToRadians(i * AngleStep);
			FVector CurrentOffset = FVector(FMath::Cos(CurrentAngle) * Radius, FMath::Sin(CurrentAngle) * Radius, 0.0f);

			FVector CurrentTopPoint = TopCenter + Rotation.RotateVector(CurrentOffset);
			FVector CurrentBottomPoint = BottomCenter + Rotation.RotateVector(CurrentOffset);

			// Draw the lines connecting the top and bottom points (red lines)
			DrawLine(World, Proxy, CurrentTopPoint, CurrentBottomPoint, Color, bPersistentLines, LifeTime, Thickness);
		}
	}
#endif
}

void UTetherDrawing::DrawPipe(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
	const FVector& OuterDimensions, float ArcAngle, const FRotator& Rotation, const FColor& Color,
	bool bPersistentLines, float LifeTime, float Thickness)
{
#if ENABLE_DRAW_DEBUG
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
        DrawLine(World, Proxy, OuterBottomPoints[i], OuterTopPoints[i], Color, bPersistentLines, LifeTime, Thickness);
        DrawLine(World, Proxy, InnerBottomPoints[i], InnerTopPoints[i], Color, bPersistentLines, LifeTime, Thickness);

        // Draw the horizontal lines connecting the outer and inner layers at the bottom and top
        if (i > 0)
        {
            DrawLine(World, Proxy, OuterBottomPoints[i], OuterBottomPoints[i - 1], Color, bPersistentLines, LifeTime, Thickness);
            DrawLine(World, Proxy, InnerBottomPoints[i], InnerBottomPoints[i - 1], Color, bPersistentLines, LifeTime, Thickness);
            DrawLine(World, Proxy, OuterTopPoints[i], OuterTopPoints[i - 1], Color, bPersistentLines, LifeTime, Thickness);
            DrawLine(World, Proxy, InnerTopPoints[i], InnerTopPoints[i - 1], Color, bPersistentLines, LifeTime, Thickness);

            // Draw the diagonal connecting lines between the outer and inner layers (caps)
            DrawLine(World, Proxy, OuterBottomPoints[i], InnerBottomPoints[i], Color, bPersistentLines, LifeTime, Thickness);
            DrawLine(World, Proxy, OuterTopPoints[i], InnerTopPoints[i], Color, bPersistentLines, LifeTime, Thickness);
        }
    }

    // Close the arc if the angle is less than 360 degrees
    if (ArcAngle < 360.f)
    {
        DrawLine(World, Proxy, OuterBottomPoints[0], InnerBottomPoints[0], Color, bPersistentLines, LifeTime, Thickness);
        DrawLine(World, Proxy, OuterTopPoints[0], InnerTopPoints[0], Color, bPersistentLines, LifeTime, Thickness);
        DrawLine(World, Proxy, OuterBottomPoints[NumSegments], InnerBottomPoints[NumSegments], Color, bPersistentLines, LifeTime, Thickness);
        DrawLine(World, Proxy, OuterTopPoints[NumSegments], InnerTopPoints[NumSegments], Color, bPersistentLines, LifeTime, Thickness);
    }
#endif
}

void UTetherDrawing::DrawCircle(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float Radius,
	int32 Segments, const FColor& Color, const FVector& YAxis, const FVector& ZAxis, bool bPersistentLines,
	float LifeTime, float Thickness)
{
#if ENABLE_DRAW_DEBUG
	// Ensure there is either a Proxy or a World
	if (!Proxy && !World)
	{
		return;
	}

	// Ensure that we have at least 4 segments for a valid circle
	Segments = FMath::Max(Segments, 4);
	const float AngleStep = 2.f * UE_PI / (float)Segments;

	FMatrix TransformMatrix;
	TransformMatrix.SetOrigin(Center);
	TransformMatrix.SetAxis(0, FVector::ForwardVector);
	TransformMatrix.SetAxis(1, YAxis);
	TransformMatrix.SetAxis(2, ZAxis);

	// Get the circle center and axes from the transformation matrix
	const FVector Origin = TransformMatrix.GetOrigin();
	const FVector AxisY = TransformMatrix.GetScaledAxis(EAxis::Y);
	const FVector AxisZ = TransformMatrix.GetScaledAxis(EAxis::Z);

	float Angle = 0.f;

	// We will manually draw the circle using line segments
	for (int32 i = 0; i < Segments; ++i)
	{
		// Calculate the start and end points of the line segment
		const FVector Vertex1 = Origin + Radius * (AxisY * FMath::Cos(Angle) + AxisZ * FMath::Sin(Angle));
		Angle += AngleStep;
		const FVector Vertex2 = Origin + Radius * (AxisY * FMath::Cos(Angle) + AxisZ * FMath::Sin(Angle));

		// Draw the line using the Proxy if it exists, otherwise use the World
		if (Proxy)
		{
			Proxy->AnimDrawDebugLine(Vertex1, Vertex2, Color, bPersistentLines, LifeTime, Thickness);
		}
		else if (World)
		{
			DrawDebugLine(World, Vertex1, Vertex2, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
		}
	}
#endif
}

void UTetherDrawing::DrawRotationGizmo(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
	const FQuat& Rotation, const FVector& AngularVelocity, const float Radius, float ArrowSize,
	int32 Segments, const FColor& VelocityColor, const FColor& XAxisColor, const FColor& YAxisColor,
	const FColor& ZAxisColor, bool bPersistentLines, float LifeTime, float Thickness)
{
#if ENABLE_DRAW_DEBUG
	// Generate a transform from the rotation to create stable axes
	FTransform RotationTransform(Rotation);

	// Use the transformed axes for drawing the gizmo
	FVector XAxis = RotationTransform.TransformVector(FVector::ForwardVector);  // X-axis (local forward)
	FVector YAxis = RotationTransform.TransformVector(FVector::RightVector);    // Y-axis (local right)
	FVector ZAxis = RotationTransform.TransformVector(FVector::UpVector);       // Z-axis (local up)

	// Now draw the rotation gizmo circles using stable axes
	DrawCircle(World, Proxy, Center, Radius, Segments, XAxisColor, XAxis, YAxis, bPersistentLines, LifeTime, Thickness); // X-axis circle
	DrawCircle(World, Proxy, Center, Radius, Segments, YAxisColor, YAxis, ZAxis, bPersistentLines, LifeTime, Thickness); // Y-axis circle
	DrawCircle(World, Proxy, Center, Radius, Segments, ZAxisColor, ZAxis, XAxis, bPersistentLines, LifeTime, Thickness); // Z-axis circle

	// Optionally, draw an arrow showing the angular velocity direction
	if (!AngularVelocity.IsNearlyZero() && ArrowSize > 0.f)
	{
		FVector EndLocation = Center + AngularVelocity.GetSafeNormal() * Radius;
		DrawArrow(World, Proxy, Center, EndLocation, VelocityColor, ArrowSize, bPersistentLines, LifeTime, Thickness);
	}
#endif
}

UCanvas* UTetherDrawing::GetDefaultCanvas()
{
	if (GEngine && GEngine->GameViewport)
	{
		static FName CanvasName(TEXT("CanvasObject"));

		// Cache to avoid FString/FName conversions/compares
		static TMap<FName, UCanvas*> CanvasMap;
		UCanvas** FoundCanvas = CanvasMap.Find(CanvasName);
		if (!FoundCanvas)
		{
			UCanvas* CanvasObject = FindObject<UCanvas>(GetTransientPackage(),*CanvasName.ToString());
			if (!CanvasObject)
			{
				CanvasObject = NewObject<UCanvas>(GetTransientPackage(), CanvasName);
				CanvasObject->AddToRoot();
			}

			CanvasMap.Add(CanvasName, CanvasObject);
			return CanvasObject;
		}

		return *FoundCanvas;
	}

	return nullptr;
}

#if WITH_EDITOR
FEditorViewportClient* UTetherDrawing::GetActiveEditorViewportClient()
{
	if (GIsEditor && GEditor->GetActiveViewport())
	{
		for (FEditorViewportClient* ViewportClient : GEditor->GetAllViewportClients())
		{
			if (ViewportClient->Viewport == GEditor->GetActiveViewport())
			{
				return ViewportClient;
			}
		}
	}
	return nullptr;
}
#endif

bool UTetherDrawing::DrawText(const FString& DebugText, TArray<FTetherDebugText>* DebugTextArray,
	const FTetherShape* Shape, const FVector& WorldLocation, FColor TextColor, UFont* Font, float FontScale,
	bool bDrawShadow)
{
#if ENABLE_DRAW_DEBUG
	if (!DebugTextArray)
	{
		return false;
	}

	if (!Shape)
	{
		return false;
	}

	// Set a default color
	if (TextColor == FColor::Transparent)
	{
		TextColor = FColor::White;
	}
	
	FTetherDebugText DebugTextInfo {};
	DebugTextInfo.Shape = Shape;
	DebugTextInfo.WorldLocation = WorldLocation;
	DebugTextInfo.Text = DebugText;
	DebugTextInfo.Color = TextColor;
	DebugTextInfo.Font = Font ? Font : GEngine->GetSmallFont();
	DebugTextInfo.FontScale = FontScale;
	DebugTextInfo.bDrawShadow = bDrawShadow;

	DebugTextArray->Add(DebugTextInfo);

	return true;
#endif
}

void UTetherDrawing::ProcessText(TArray<FTetherDebugText>* DebugTextArray, const UWorld* World, UCanvas* Canvas)
{
#if ENABLE_DRAW_DEBUG
	if (!IsInGameThread())
	{
		return;
	}

	if (!DebugTextArray)
	{
		return;
	}
	
	if (!Canvas)
	{
		return;
	}

	// Determine the camera transform
	FVector CameraLoc;
	FRotator CameraRot;
	if (APlayerController* PlayerController = World ? GEngine->GetFirstLocalPlayerController(World) : nullptr)
	{
		PlayerController->GetPlayerViewPoint(CameraLoc, CameraRot);
	}
	else
	{
#if WITH_EDITOR
		if (FEditorViewportClient* ViewportClient = GetActiveEditorViewportClient())
		{
			CameraLoc = ViewportClient->GetViewLocation();
			CameraRot = ViewportClient->GetViewRotation();
		}
#else
		return;
#endif
	}

	// Process text
	for (const FTetherDebugText& DebugText : *DebugTextArray)
	{
		// don't draw text behind the camera
		if (((DebugText.WorldLocation - CameraLoc) | CameraRot.Vector()) > 0.f)
		{
			// Determine the screen position of the text
			FVector ScreenProject = Canvas->Project(DebugText.WorldLocation);
			FVector2D ScreenPosition { FMath::CeilToFloat(ScreenProject.X), FMath::CeilToFloat(ScreenProject.Y) };
			
			FCanvasTextItem TextItem { ScreenPosition, FText::FromString(DebugText.Text), DebugText.Font, DebugText.Color };

			if (DebugText.bDrawShadow)
			{
				TextItem.EnableShadow(FLinearColor::Black);
			}
			else
			{
				TextItem.DisableShadow();
			}

			TextItem.Scale = FVector2D { DebugText.FontScale };

			Canvas->DrawItem(TextItem, ScreenPosition);
		}
	}

	// Reset the array
	DebugTextArray->Reset();
#endif
}
