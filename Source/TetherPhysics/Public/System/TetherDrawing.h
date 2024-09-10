// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherDrawing.generated.h"

enum class ETetherActivityState : uint8;
enum class ETetherSimulationMode : uint8;
struct FTetherShape;
struct FTetherDebugText;

UCLASS()
class TETHERPHYSICS_API UTetherDrawing : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void DrawLine(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	static void DrawPoint(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, const FColor& Color, float Size = 4.f, bool bPersistentLines = false, float LifeTime = -1.f);
	static void DrawArrow(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End, const FColor& Color, float ArrowSize = 40.f, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	static void DrawBox(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, const FVector& Extent, const FQuat& Rotation, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	static void DrawSphere(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float Radius, int32 Segments, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	static void DrawCapsule(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float HalfHeight, float Radius, const FRotator& Rotation, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	static void DrawPipe(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, const FVector& OuterDimensions, float ArcAngle, const FRotator& Rotation, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	static void DrawCircle(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float Radius, int32 Segments, const FColor& Color, const FVector& YAxis = FVector::RightVector, const FVector& ZAxis = FVector::UpVector, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	
	static void DrawRotationGizmo(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
		const FQuat& Rotation, const FVector& AngularVelocity, const float Radius = 16.f, float ArrowSize = 24.f,
		int32 Segments = 32, const FColor& VelocityColor = FColor::Orange, const FColor& XAxisColor = FColor::Red,
		const FColor& YAxisColor = FColor::Green, const FColor& ZAxisColor = FColor::Blue,
		bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);

public:
	static FString SimulationModeString(ETetherSimulationMode Mode);
	static FString GetActivityStateString(ETetherActivityState State);
	
public:
	static UCanvas* GetDefaultCanvas();

#if WITH_EDITOR
	static FEditorViewportClient* GetActiveEditorViewportClient();
#endif

	/** @return True if debug text was added */
	static bool DrawText(const FString& DebugText, TArray<FTetherDebugText>* DebugTextArray,
		const FTetherShape* Shape = nullptr, const FVector& WorldLocation = FVector::ZeroVector,
		FColor TextColor = FColor::Transparent, UFont* Font = nullptr, float FontScale = 1.f, bool bDrawShadow = false);

	/** Called when drawing to Canvas to actually display text processed in DrawText() */
	static void ProcessText(TArray<FTetherDebugText>* DebugTextArray, const UWorld* World, UCanvas* Canvas);
};