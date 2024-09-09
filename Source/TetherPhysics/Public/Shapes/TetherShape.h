// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TetherPhysicsTypes.h"
#include "UObject/Object.h"
#include "TetherShape.generated.h"

struct FTetherShape_AxisAlignedBoundingBox;
class UTetherShapeObject;

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

	/** Defines how a physics object is controlled within the Tether physics engine. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	ETetherSimulationMode SimulationMode = ETetherSimulationMode::Simulated;

	/** Defines the wake/sleep state of a physics object, determining whether it's actively simulated or idle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	ETetherActivityState ActivityState = ETetherActivityState::Awake;
	
	/**
	 * Typically represents the spatial grid bucket where the shape is located, used in broad-phase collision detection
	 * This may be any kind of index in a non-spatial hashing system
	 */
	UPROPERTY()
	int32 HashIndex = 0;

	/** Cache local space data to avoid precision or rounding data loss over time */
	TSharedPtr<FTetherShape> LocalSpaceData = nullptr;

	/** Shapes that should be ignored during collision detection @TODO actually implement this */
	UPROPERTY()
	TArray<TWeakObjectPtr<UTetherShapeObject>> IgnoredShapes;

	/** Tags to categorize shapes that should be ignored during collision detection @TODO actually implement this */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FGameplayTagContainer IgnoredShapeTypes;

	UTetherShapeObject* GetTetherShapeObject() const;
	FGameplayTag GetShapeType() const;
	static FGameplayTag StaticShapeType() { return FGameplayTag::EmptyTag; }

	FVector GetCenter() const;
	
	bool IsValid() const;

	bool IsIgnored(const FTetherShape& Other) const;

	/** Checks if two shapes are set to ignore each other */
	static bool AreShapesIgnoringEachOther(const FTetherShape& ShapeA, const FTetherShape& ShapeB);

	/** Returns whether the shape is currently in world space */
	bool IsWorldSpace() const { return bWorldSpace; }

	/** Converts the shape's data to world space using the given transformation */
	void ToWorldSpace(const FTransform& InWorldTransform);

	/** Converts the shape's data back to local space */
	void ToLocalSpace();

	/** Returns the shape's world transformation */
	const FTransform& GetWorldTransform() const { return WorldTransform; }

protected:
	/** The transformation that was applied to the shape when converting to world space */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	FTransform WorldTransform = FTransform::Identity;

	/** Indicates whether the shape is currently in world space */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	bool bWorldSpace = false;

public:
	/** Draws the shape for debugging purposes using the animation instance proxy */
	void DrawDebug(UWorld* World, FAnimInstanceProxy* Proxy, const FColor& Color = FColor::Red,
		bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f) const;
};

/**
 * A simple struct representing a pair of shapes for collision detection.
 *
 * FTetherShapePair is used to store the indices of two shapes that are to be tested for collisions
 * in the physics simulation. It includes a method to check if two pairs are equal, regardless of the order
 * of the shapes, making it useful for identifying and managing unique collision pairs.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShapePair
{
	GENERATED_BODY()
	
	/** Index of the first shape in the pair */
	int32 ShapeIndexA;

	/** Index of the second shape in the pair */
	int32 ShapeIndexB;

	FTetherShapePair()
		: ShapeIndexA(INDEX_NONE)
		, ShapeIndexB(INDEX_NONE)
	{}

	/** 
	 * Constructor to initialize the pair with the indices of two shapes 
	 * @param InShapeIndexA Index of the first shape
	 * @param InShapeIndexB Index of the second shape
	 */
	FTetherShapePair(int32 InShapeIndexA, int32 InShapeIndexB)
		: ShapeIndexA(InShapeIndexA)
		, ShapeIndexB(InShapeIndexB)
	{}

	bool ContainsShape(int32 ShapeIndex) const
	{
		return ShapeIndexA == ShapeIndex || ShapeIndexB == ShapeIndex;
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
		return (ShapeIndexA == Other.ShapeIndexA && ShapeIndexB == Other.ShapeIndexB) ||
			   (ShapeIndexA == Other.ShapeIndexB && ShapeIndexB == Other.ShapeIndexA);
	}
};

/**
 * Base class for defining behavior and virtual functions for tether shapes.
 *
 * UTetherShapeObject provides the functionality needed for tether shapes that
 * go beyond the data stored in FTetherShape. Since USTRUCTs don't support polymorphism,
 * this class is used to override functions, handle complex behaviors, and manage
 * operations like transformation and drawing that require virtual methods.
 *
 * This class is typically subclassed to define specific behaviors for different types of shapes
 * in the physics system. All functions in this class should be `const` as the object is
 * generally used in a read-only context to manage tether shapes.
 */
UCLASS(Const, Abstract, NotBlueprintable, NotBlueprintType)
class TETHERPHYSICS_API UTetherShapeObject : public UObject
{
	GENERATED_BODY()

public:
	/** Returns the gameplay tag that identifies the type of shape */
	virtual FGameplayTag GetShapeType() const { return FGameplayTag::EmptyTag; }

	/** Returns the center of the shape in local space */
	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const { return FVector::ZeroVector; }

	/** Transforms the shape data from local space to world space */
	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const {}
	
	/** Transforms the shape data from world space back to local space */
	virtual void TransformToLocalSpace(FTetherShape& Shape) const {}

	/** Gets the shape as a bounding box */
	virtual FTetherShape_AxisAlignedBoundingBox GetBoundingBox(const FTetherShape& Shape) const;

	/** Gets the shape identifier for debugging purposes */
	virtual FString GetShapeDebugString() const { return GetShapeType().ToString(); }

	/** Draws the shape for debugging purposes */
	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* Proxy = nullptr,
		UWorld* World = nullptr, const FColor& Color = FColor::Red, bool bPersistentLines = false,
		float LifeTime = -1.f, float Thickness = 0.f) const {}
};
