// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_BoundingSphere.generated.h"

/**
 * The Bounding Sphere is extremely simple to compute. It is defined by a center point and a radius, which can be
 * derived from the furthest point from the center of the object.
 *
 * Collision detection between spheres (and between a sphere and other shapes) is very straightforward,
 * involving basic distance checks.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape_BoundingSphere : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_BoundingSphere();

	FTetherShape_BoundingSphere(const FVector& InCenter, float InRadius);

	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_BoundingSphere>(*this); }

	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_BoundingSphere; }

	void ToLocalSpace_Implementation();
	
	/** Center of the sphere */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Center;

	/** Radius of the sphere */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float Radius;
};

UCLASS()
class TETHERPHYSICS_API UTetherShapeObject_BoundingSphere : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_BoundingSphere;	}

	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const override;
	
	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
