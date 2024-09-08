// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherStatics.generated.h"

struct FTetherDebugText;
struct FTetherShape;
/**
 * Declares a custom stats group for the Tether system, used for profiling and performance tracking.
 * 
 * This allows you to run "stat tether" to gather and view performance data related to the Tether system within
 * Unreal Engine's profiling tools.
 */
DECLARE_STATS_GROUP(TEXT("Tether_Stat"), STATGROUP_Tether, STATCAT_Advanced); 

/** Logging of messages related to Tether. */
DEFINE_LOG_CATEGORY_STATIC(LogTether, Log, All);

namespace FTether
{
	/**
	 * The moment of inertia is a measure of an object's resistance to rotation, influenced by its shape and mass distribution.
	 *
	 * For a box-shaped object, the moment of inertia is calculated using the box's mass and its dimensions. The constant
	 * 12.0f is derived from the mathematical formula used to determine how the mass is distributed relative to the axis of rotation.
	 * This constant ensures that the influence of the box's dimensions on its rotational inertia is correctly scaled.
	 */
	static constexpr float MomentOfInertia = 12.f;
}

UCLASS()
class TETHERPHYSICS_API UTetherStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void DrawLine(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	static void DrawArrow(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End, const FColor& Color, float ArrowSize = 40.f, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	static void DrawBox(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, const FVector& Extent, const FQuat& Rotation, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	static void DrawCircle(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float Radius, int32 Segments, const FColor& Color, const FVector& YAxis = FVector::RightVector, const FVector& ZAxis = FVector::UpVector, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	
	static void DrawRotationGizmo(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
		const FQuat& Rotation, const FVector& AngularVelocity, const float Radius = 16.f, float ArrowSize = 24.f,
		int32 Segments = 32, const FColor& VelocityColor = FColor::Orange, const FColor& XAxisColor = FColor::Red,
		const FColor& YAxisColor = FColor::Green, const FColor& ZAxisColor = FColor::Blue,
		bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);
	
public:
	static UCanvas* GetDebugCanvas();

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