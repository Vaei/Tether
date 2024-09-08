// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherStatics.h"

#include "TetherPhysicsTypes.h"
#include "Engine/Canvas.h"
#include "Shapes/TetherShape.h"

#include "DrawDebugHelpers.h"
#include "Animation/AnimInstanceProxy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherStatics)

void UTetherStatics::DrawLine(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End,
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

void UTetherStatics::DrawArrow(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End,
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

void UTetherStatics::DrawBox(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
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

UCanvas* UTetherStatics::GetDebugCanvas()
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

bool UTetherStatics::AddDebugText(const FString& DebugText, TArray<FTetherDebugText>* DebugTextArray,
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
	
	UCanvas* Canvas = GetDebugCanvas();
	if (!Canvas)
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

void UTetherStatics::DrawDebugText(TArray<FTetherDebugText>* DebugTextArray, const UWorld* World)
{
	if (!IsInGameThread())
	{
		return;
	}

	if (!DebugTextArray)
	{
		return;
	}
	
	if (!World)
	{
		return;
	}

	UCanvas* Canvas = GetDebugCanvas();
	if (!Canvas)
	{
		return;
	}

	FVector CameraLoc;
	FRotator CameraRot;
	if (APlayerController* PC = GEngine->GetFirstLocalPlayerController(World))
	{
		PC->GetPlayerViewPoint(CameraLoc, CameraRot);
	}
	else
	{
		return;
	}
	
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

	DebugTextArray->Reset();
}
