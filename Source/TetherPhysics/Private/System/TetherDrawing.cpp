// Copyright (c) Jared Taylor. All Rights Reserved.


#include "System/TetherDrawing.h"

#include "Animation/AnimInstanceProxy.h"
#include "Engine/Canvas.h"
#include "System/TetherMessaging.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherDrawing)

void UTetherDrawing::DrawLine(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End,
	const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness)
{
	if (Proxy)
	{
		Proxy->AnimDrawDebugLine(Start, End, Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)
	{
		DrawDebugLine(World, Start, End, Color, bPersistentLines, LifeTime, 0, Thickness);
	}
}

void UTetherDrawing::DrawArrow(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End,
	const FColor& Color, float ArrowSize, bool bPersistentLines, float LifeTime, float Thickness)
{
	if (Proxy)
	{
		Proxy->AnimDrawDebugDirectionalArrow(Start, End, ArrowSize, Color, bPersistentLines, LifeTime, Thickness);
	}
	else if (World)
	{
		DrawDebugDirectionalArrow(World, Start, End, ArrowSize, Color, bPersistentLines, LifeTime, 0, Thickness);
	}
}

void UTetherDrawing::DrawBox(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
	const FVector& Extent, const FQuat& Rotation, const FColor& Color, bool bPersistentLines, float LifeTime,
	float Thickness)
{
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
}

void UTetherDrawing::DrawCircle(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float Radius,
	int32 Segments, const FColor& Color, const FVector& YAxis, const FVector& ZAxis, bool bPersistentLines,
	float LifeTime, float Thickness)
{
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
			DrawDebugLine(World, Vertex1, Vertex2, Color, bPersistentLines, LifeTime, 0, Thickness);
		}
	}
}

void UTetherDrawing::DrawRotationGizmo(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
	const FQuat& Rotation, const FVector& AngularVelocity, const float Radius, float ArrowSize,
	int32 Segments, const FColor& VelocityColor, const FColor& XAxisColor, const FColor& YAxisColor,
	const FColor& ZAxisColor, bool bPersistentLines, float LifeTime, float Thickness)
{
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
}

UCanvas* UTetherDrawing::GetDebugCanvas()
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
			if( !CanvasObject )
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
}

void UTetherDrawing::ProcessText(TArray<FTetherDebugText>* DebugTextArray, const UWorld* World, UCanvas* Canvas)
{
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
}
