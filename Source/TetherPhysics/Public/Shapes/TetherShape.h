// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "TetherShape.generated.h"

struct FTetherShape_AxisAlignedBoundingBox;
class UTetherShapeObject;

/** Defines how a physics object is controlled within the Tether physics engine. */
UENUM(BlueprintType)
enum class ETetherSimulationMode : uint8
{
	Simulated			UMETA(ToolTip="Simulated by the physics engine"),
	Kinematic			UMETA(ToolTip="Does not respond to physics forces"),
	Inertial			UMETA(ToolTip="Retains internal physics like damping but won't apply external forces"),
};

/** Defines the wake/sleep state of a physics object, determining whether it's actively simulated or idle. */
UENUM(BlueprintType)
enum class ETetherActivityState : uint8
{
	Awake				UMETA(ToolTip="Awake and actively simulated"),
	ForceAwake			UMETA(ToolTip="Awake and actively simulated - will not sleep"),
	Asleep				UMETA(ToolTip="Asleep and not simulated until disturbed"),
	ForceAsleep			UMETA(ToolTip="Asleep and not simulated even if disturbed - will not wake"),
};

/**
 * The settings and data container for a tether shape in the Tether physics system.
 *
 * FTetherShape is a lightweight struct that holds configuration data and metadata for a shape in the simulation.
 * Unlike typical USTRUCTs, it avoids direct copying to prevent loss of specific shape data, as derived types
 * may hold additional information not present in this base type. Instead, polymorphism is handled manually
 * through the `Clone()` method, which creates a copy of the object while preserving its specific type and data.
 *
 * Virtual functions and complex behaviors related to the shape are handled by a corresponding UTetherShapeObject class.
 * This design allows for extended flexibility, enabling custom shapes and behaviors to be defined while maintaining
 * the integrity and uniqueness of the shape data.
 *
 * @TODO Implement mechanisms to prevent unintended copying of this struct.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape
{
	GENERATED_BODY()

public:
	FTetherShape() = default;
	virtual ~FTetherShape() = default;

	/** 
	 * Creates a copy of the shape, preserving its specific type and data.
	 * This method must be used instead of copying the struct directly to ensure data integrity.
	 */
	virtual TSharedPtr<FTetherShape> Clone() const { return MakeShared<FTetherShape>(*this); }

public:
	/** The class that defines the shape's behavior and virtual functions */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	TSubclassOf<UTetherShapeObject> TetherShapeClass = nullptr;

	/** Defines how a physics object is controlled within the Tether physics engine */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	ETetherSimulationMode SimulationMode = ETetherSimulationMode::Simulated;

	/** Defines the wake/sleep state of a physics object, determining whether it's actively simulated or idle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	ETetherActivityState ActivityState = ETetherActivityState::Awake;

	/** @return true if the object is awake */
	bool IsAwake() const
	{
		return ActivityState == ETetherActivityState::Awake || ActivityState == ETetherActivityState::ForceAwake;
	}

	/** @return true if the object is asleep */
	bool IsAsleep() const
	{
		return !IsAwake();
	}

	/** Time remaining until the object sleeps, if not disturbed */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	float TimeUntilSleep = 0.f;

	/** Last world time a broad phase collision occurred involving this shape */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	double LastBroadCollisionTime = INFINITY;

	/** Last world time a narrow phase collision occurred involving this shape */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	double LastNarrowCollisionTime = INFINITY;

	/** Calculates the time since a specified event, accounting for infinite time values */
	static double TimeSince(double WorldTime, double Time)
	{
		if (FMath::IsNearlyEqual(Time, INFINITY)) { return INFINITY; }
		return WorldTime - Time;
	}

	/** Returns the time since the last broad phase collision occurred */
	double TimeSinceBroadCollision(double WorldTime) const
	{
		return TimeSince(WorldTime, LastBroadCollisionTime);
	}

	/** Checks if a broad phase collision occurred recently within a specified time window */
	bool HasRecentBroadCollision(double WorldTime, double Time) const
	{
		return TimeSinceBroadCollision(WorldTime) <= Time;
	}

	/** Returns the time since the last narrow phase collision occurred */
	double TimeSinceNarrowCollision(double WorldTime) const
	{
		return TimeSince(WorldTime, LastNarrowCollisionTime);
	}

	/** Checks if a narrow phase collision occurred recently within a specified time window */
	bool HasRecentNarrowCollision(double WorldTime, double Time) const
	{
		return TimeSinceNarrowCollision(WorldTime) <= Time;
	}
	
	/**
	 * Typically represents the spatial grid bucket where the shape is located, used in broad-phase collision detection
	 * This may be any kind of index in a non-spatial hashing system
	 */
	UPROPERTY()
	int32 HashIndex = 0;

	/** Cache local space data to avoid precision or rounding data loss over time */
	TSharedPtr<FTetherShape> LocalSpaceData = nullptr;

	/** Shape to be ignored during collision detection @TODO actually implement this */
	UPROPERTY()
	TArray<TWeakObjectPtr<UTetherShapeObject>> IgnoredShapes;

	/** Shape types to be ignored during collision detection @TODO actually implement this */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FGameplayTagContainer IgnoredShapeTypes;

	/** @return the shape's object that manages its virtual behavior */
	UTetherShapeObject* GetTetherShapeObject() const;

	/** @return the gameplay tag identifying the shape's type */
	FGameplayTag GetShapeType() const;

	/** @return the static tag representing the shape's type */
	static FGameplayTag StaticShapeType() { return FGameplayTag::EmptyTag; }

	/** @return the name of the shape, typically used for debugging purposes */
	FString GetName() const;

	/** @return the FName representation of the shape name */
	FName GetFName() const { return FName(GetName()); }

	/** Retrieves the center of the shape in local space */
	FVector GetLocalSpaceCenter() const;

	/** Checks if the shape is valid */
	bool IsValid() const;

	/** Checks if this shape is set to ignore collisions with another shape */
	bool IsIgnored(const FTetherShape& Other) const;

	/** Checks if two shapes are set to ignore each other */
	static bool AreShapesIgnoringEachOther(const FTetherShape& ShapeA, const FTetherShape& ShapeB);

	/** Returns whether the shape is currently in world space */
	bool IsWorldSpace() const { return bWorldSpace; }

	/** Converts the shape's data to world space using the provided world-space transform */
	void ToWorldSpace(const FTransform& InWorldTransform);

	/** Converts the shape's data back to local space */
	void ToLocalSpace();

	/** @return the shape's world transform that was applied to convert from local space */
	const FTransform& GetAppliedWorldTransform() const { return AppliedWorldTransform; }

protected:
	/** The transform that was applied to the shape when converting to world space */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	FTransform AppliedWorldTransform = FTransform::Identity;

	/** Indicates whether the shape is currently in world space */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	bool bWorldSpace = false;

public:
	/** Draws the shape for debugging purposes using the animation instance proxy */
	void DrawDebug(const UWorld* World, FAnimInstanceProxy* Proxy, const FColor& Color = FColor::Red,
		bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f) const;
};

/**
 * A struct representing a pair of shapes for collision detection.
 *
 * FTetherShapePair is used to store the indices of two shapes that are to be tested for collisions
 * in the physics simulation. It includes a method to check if two pairs are equal, regardless of the order
 * of the shapes, making it useful for identifying and managing unique collision pairs.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShapePair
{
	GENERATED_BODY()

	FTetherShape* ShapeA;
	FTetherShape* ShapeB;

	FTetherShapePair()
		: ShapeA(nullptr)
		, ShapeB(nullptr)
	{}

	/** 
	 * Constructor to initialize the pair with the indices of two shapes 
	 * @param InShapeA The first shape
	 * @param InShapeB The second shape
	 */
	FTetherShapePair(FTetherShape* InShapeA, FTetherShape* InShapeB)
		: ShapeA(InShapeA)
		, ShapeB(InShapeB)
	{}

	/** Checks if the specified shape is part of this pair */
	bool ContainsShape(const FTetherShape* Shape) const
	{
		return ShapeA == Shape || ShapeB == Shape;
	}
	
	/**
	 * Checks if two shape pairs are equal, regardless of the order of shapes.
	 * This method ensures that (A, B) is considered equal to (B, A).
	 * 
	 * @param Other The other shape pair to compare against.
	 * @return True if the pairs are equal, false otherwise.
	 */
	bool operator==(const FTetherShapePair& Other) const
	{
		return (ShapeA == Other.ShapeA && ShapeB == Other.ShapeB) ||
			   (ShapeA == Other.ShapeB && ShapeB == Other.ShapeA);
	}
};

/**
 * Base class for defining behavior and virtual functions for tether shapes.
 *
 * UTetherShapeObject handles complex behaviors that can't be stored in FTetherShape.
 * This class is subclassed to define specific behaviors for various types of shapes,
 * including how they are transformed and drawn for debugging purposes.
 */
UCLASS(Const, Abstract, NotBlueprintable, NotBlueprintType)
class TETHERPHYSICS_API UTetherShapeObject : public UObject
{
	GENERATED_BODY()

public:
	/** @return the gameplay tag that identifies the type of shape */
	virtual FGameplayTag GetShapeType() const { return FGameplayTag::EmptyTag; }

	/** @return the center of the shape in local space */
	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const { return FVector::ZeroVector; }

	/** Transforms the shape data from local space to world space */
	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const {}
	
	/** Transforms the shape data from world space back to local space */
	virtual void TransformToLocalSpace(FTetherShape& Shape) const {}

	/** @return the bounding box of the shape */
	virtual FTetherShape_AxisAlignedBoundingBox GetBoundingBox(const FTetherShape& Shape) const;

	/** @return the shape identifier for debugging purposes */
	virtual FString GetShapeDebugString() const { return GetShapeType().ToString(); }

	/** Draws the shape for debugging purposes */
	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy = nullptr,
		const UWorld* World = nullptr, const FColor& Color = FColor::Red, bool bPersistentLines = false,
		float LifeTime = -1.f, float Thickness = 0.f) const {}
};
