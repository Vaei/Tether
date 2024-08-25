// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "TetherShape.generated.h"

class UTetherShapeObject;

/**
 * The settings for a tether shape
 * Virtual functions are handled by the UTetherShapeObject instead
 */
USTRUCT(BlueprintType)
struct TETHER_API FTetherShape
{
	GENERATED_BODY()

	FTetherShape();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	TSubclassOf<UTetherShapeObject> TetherShapeClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	TArray<TWeakObjectPtr<UTetherShapeObject>> IgnoredShapes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FGameplayTagContainer IgnoredShapeTypes;

	UTetherShapeObject* GetTetherShape() const;
	FGameplayTag GetShapeType() const;
	static FGameplayTag StaticShapeType() { return FGameplayTag::EmptyTag; }
	
	bool IsValid() const;

	bool IsIgnored(const FTetherShape& Other) const;

	/** Checks if two shapes are ignoring each other */
	static bool AreShapesIgnoringEachOther(const FTetherShape& ShapeA, const FTetherShape& ShapeB);

	bool IsWorldSpace() const { return bInWorldSpace; }
	
	void ToWorldSpace(const FTransform& InWorldTransform)
	{
		bInWorldSpace = true;
		WorldTransform = InWorldTransform;
	}

	void ToLocalSpace()
	{
		bInWorldSpace = false;
	}

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

	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const {}
	virtual void TransformToLocalSpace(FTetherShape& Shape) const {}

	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color,
		bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f) const {}
};
