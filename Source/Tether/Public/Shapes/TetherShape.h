// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

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
 * We prevent copying because :
 *		There is no need to copy this base type as it isn't used - other than for passing around
 *		We do not pass around derived types, we only pass this type
 *		We don't know what derived types may or may not exist
 *		If we try to copy any arbitrary type to a FTetherShape, we lose all the data pertaining to the specific shape
 *		FTetherShape::Clone() will give you an FTetherShape with the data intact
 */
USTRUCT(BlueprintType)
struct TETHER_API FTetherShape
{
	GENERATED_BODY()

	FTetherShape()
	{}
	virtual ~FTetherShape() = default;

protected:
	// Allow copy constructor but make it protected
	FTetherShape(const FTetherShape&) = default;
	FTetherShape& operator=(const FTetherShape&) = default;

public:
	/** Implement move constructor and assignment operator */
	FTetherShape(FTetherShape&&) = default;
	FTetherShape& operator=(FTetherShape&&) = default;

	/** Add a virtual clone method - this must be used instead of copying! */
	virtual TSharedPtr<FTetherShape> Clone() const { return MakeShared<FTetherShape>(*this); }
	
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	TSubclassOf<UTetherShapeObject> TetherShapeClass = nullptr;

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

	void DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color = FColor::Red,
		bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f) const;

	void DrawDebug(UWorld* World, const FColor& Color = FColor::Red, bool bPersistentLines = false,
		float LifeTime = -1.f, float Thickness = 0.f) const;
	
	bool IsWorldSpace() const { return bInWorldSpace; }
	
	void ToWorldSpace(const FTransform& InWorldTransform);

	void ToLocalSpace();

	const FTransform& GetWorldTransform() const { return WorldTransform; }

protected:
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	FTransform WorldTransform = FTransform::Identity;

	UPROPERTY(BlueprintReadOnly, Category=Tether)
	bool bInWorldSpace = false;
};

/**
 * This doesn't store any data as a rule, and is used as const - all functions must be const
 * It exists for the purpose of overriding functions, as USTRUCTs don't have vtables/polymorphism
 */
UCLASS(Const, Abstract, NotBlueprintable, NotBlueprintType)
class TETHER_API UTetherShapeObject : public UObject
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
