// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherDrawing.generated.h"

enum class ETetherActivityState : uint8;
enum class ETetherSimulationMode : uint8;
struct FTetherShape;
struct FTetherDebugText;

/**
 * A utility class for drawing debug shapes and text in the Tether physics system.
 * UTetherDrawing is used for visualizing various components of the physics simulation,
 * such as shapes, velocities, and other important details in both the editor and game worlds.
 * It also contains helper functions for converting simulation modes and activity states into strings.
 */
UCLASS()
class TETHERPHYSICS_API UTetherDrawing : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Draws a line between two points in the world. */
	static void DrawLine(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);

	/** Draws a point at the specified location. */
	static void DrawPoint(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, const FColor& Color, float Size = 4.f, bool bPersistentLines = false, float LifeTime = -1.f);

	/** Draws an arrow between two points in the world, useful for showing direction and velocity. */
	static void DrawArrow(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Start, const FVector& End, const FColor& Color, float ArrowSize = 40.f, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);

	/** Draws a box at the specified location with the given extents and rotation. */
	static void DrawBox(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, const FVector& Extent, const FQuat& Rotation, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);

	/** Draws a sphere at the specified location with the given radius. */
	static void DrawSphere(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float Radius, int32 Segments, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);

	/** Draws a capsule shape at the specified location with the given dimensions and rotation. */
	static void DrawCapsule(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float HalfHeight, float Radius, const FRotator& Rotation, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);

	/** Draws a circle at the specified location, defined by the radius and axes for alignment. */
	static void DrawCircle(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center, float Radius, int32 Segments, const FColor& Color, const FVector& YAxis = FVector::RightVector, const FVector& ZAxis = FVector::UpVector, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);

	/**
	 * Draws a rotation gizmo at the specified location to visualize the rotation and angular velocity of an object.
	 * It can also display axes (X, Y, Z) in different colors and an optional velocity arrow.
	 */
	static void DrawRotationGizmo(const UWorld* World, FAnimInstanceProxy* Proxy, const FVector& Center,
		const FQuat& Rotation, const FVector& AngularVelocity, const float Radius = 16.f, float ArrowSize = 24.f,
		int32 Segments = 32, const FColor& VelocityColor = FColor::Orange, const FColor& XAxisColor = FColor::Red,
		const FColor& YAxisColor = FColor::Green, const FColor& ZAxisColor = FColor::Blue,
		bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f);

public:

	/** Converts a simulation mode enum to its string representation. */
	static FString SimulationModeString(ETetherSimulationMode Mode);

	/** Converts an activity state enum to its string representation. */
	static FString GetActivityStateString(ETetherActivityState State);

public:
#if WITH_EDITOR
	/** Returns a reference to the currently active editor viewport client, used for rendering debug information in the editor. */
	static FEditorViewportClient* GetActiveEditorViewportClient();
#endif

	/**
	 * Draws text at the specified location, usually for displaying debug information.
	 * @return True if the text was successfully added to the debug array.
	 */
	static bool DrawText(const FString& DebugText, TArray<FTetherDebugText>* DebugTextArray,
		const FTetherShape* Shape = nullptr, const FVector& WorldLocation = FVector::ZeroVector,
		FColor TextColor = FColor::Transparent, UFont* Font = nullptr, float FontScale = 1.f, bool bDrawShadow = false);

	/**
	 * Processes and displays the accumulated debug text in the array on the canvas.
	 * This method is called during canvas rendering to actually draw the debug information.
	 */
	static void ProcessText(TArray<FTetherDebugText>* DebugTextArray, const UWorld* World, UCanvas* Canvas);
};
