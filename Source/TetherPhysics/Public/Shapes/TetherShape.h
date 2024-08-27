// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "TetherShape.generated.h"

class UTetherShapeObject;

/**
 * The settings and data container for a tether shape.
 * 
 * Virtual functions are handled by the UTetherShapeObject instead.
 *
 * Clone() is the exception, effectively implementing manual polymorphism.
 *
 * @TODO actually prevent copying
 * We prevent copying because :
 *		There is no need to copy this base type as it isn't used - other than for passing around
 *		We do not pass around derived types, we only pass this type
 *		We don't know what derived types may or may not exist
 *		If we try to copy any arbitrary type to a FTetherShape, we lose all the data pertaining to the specific shape
 *		FTetherShape::Clone() will give you an FTetherShape with the data intact
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape
{
	GENERATED_BODY()

public:
	FTetherShape() = default;
	virtual ~FTetherShape() = default;

	/** Add a virtual clone method - this must be used instead of copying! */
	virtual TSharedPtr<FTetherShape> Clone() const { return MakeShared<FTetherShape>(*this); }

public:
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	TSubclassOf<UTetherShapeObject> TetherShapeClass = nullptr;

	/**
	 * The Shape with the higher efficiency rating will be evaluated in a pairing
	 * 
	 * Some pairings are potentially asymmetric during the narrow phase and can be more expensive
	 * depending on the order of calculations
	 *
	 * You can compute the time it takes to evaluate by having ChatGPT convert the method to python
	 * and evaluate how long it takes to run, but of course you will have to run all functions
	 * for comparison. @see UTetherShapeCollisionControl.
	 */
	UPROPERTY()
	uint8 EfficiencyRating = 0;  // @todo implement for all shapes

	/** The bucket where we exist on the spatial grid */
	UPROPERTY()
	int32 Bucket = 0;

	/** Cache local space data to avoid precision or rounding data loss over time */
	TSharedPtr<FTetherShape> LocalSpaceData = nullptr;

	UPROPERTY()
	TArray<TWeakObjectPtr<UTetherShapeObject>> IgnoredShapes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FGameplayTagContainer IgnoredShapeTypes;

	UTetherShapeObject* GetTetherShape() const;
	FGameplayTag GetShapeType() const;
	static FGameplayTag StaticShapeType() { return FGameplayTag::EmptyTag; }

	FVector GetCenter() const;
	
	bool IsValid() const;

	bool IsIgnored(const FTetherShape& Other) const;

	/** Checks if two shapes are ignoring each other */
	static bool AreShapesIgnoringEachOther(const FTetherShape& ShapeA, const FTetherShape& ShapeB);

	bool IsWorldSpace() const { return bInWorldSpace; }
	
	void ToWorldSpace(const FTransform& InWorldTransform);

	void ToLocalSpace();

	const FTransform& GetWorldTransform() const { return WorldTransform; }

protected:
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	FTransform WorldTransform = FTransform::Identity;

	UPROPERTY(BlueprintReadOnly, Category=Tether)
	bool bInWorldSpace = false;

public:
	void DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color = FColor::Red,
		bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f) const;

	void DrawDebug(UWorld* World, const FColor& Color = FColor::Red, bool bPersistentLines = false,
		float LifeTime = -1.f, float Thickness = 0.f) const;
};

struct TETHERPHYSICS_API FTetherShapePair
{
	int32 ShapeIndexA;  // Index of the first shape in the pair
	int32 ShapeIndexB;  // Index of the second shape in the pair

	// Constructor to initialize the pair
	FTetherShapePair(int32 InShapeIndexA, int32 InShapeIndexB)
		: ShapeIndexA(InShapeIndexA), ShapeIndexB(InShapeIndexB) {}
    
	// Method to check equality between two shape pairs
	bool operator==(const FTetherShapePair& Other) const
	{
		return (ShapeIndexA == Other.ShapeIndexA && ShapeIndexB == Other.ShapeIndexB) ||
			   (ShapeIndexA == Other.ShapeIndexB && ShapeIndexB == Other.ShapeIndexA);
	}
};

/**
 * This doesn't store any data as a rule, and is used as const - all functions must be const
 * It exists for the purpose of overriding functions, as USTRUCTs don't have vtables/polymorphism
 */
UCLASS(Const, Abstract, NotBlueprintable, NotBlueprintType)
class TETHERPHYSICS_API UTetherShapeObject : public UObject
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetShapeType() const { return FGameplayTag::EmptyTag; }

	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const { return FVector::ZeroVector; }

	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const {}
	virtual void TransformToLocalSpace(FTetherShape& Shape) const {}

	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy = nullptr,
		UWorld* World = nullptr, const FColor& Color = FColor::Red, bool bPersistentLines = false,
		float LifeTime = -1.f, float Thickness = 0.f) const {}
};
