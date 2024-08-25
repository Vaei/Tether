// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_AxisAlignedBoundingBox.h"
#include "TetherShape_Capsule.generated.h"

USTRUCT(BlueprintType)
struct TETHER_API FTetherShape_Capsule : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_Capsule();

	FTetherShape_Capsule(const FVector& InCenter, float InHalfHeight, float InRadius, const FRotator& InRotation);

	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_Capsule>(*this); }

	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_Capsule; }

	/** Center of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Center;

	/** Half-height of the capsule (from the center to one of the hemispherical ends) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float HalfHeight;

	/** Radius of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float Radius;

	/** Rotation of the capsule */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FRotator Rotation;
	
	FTetherShape_AxisAlignedBoundingBox GetBoundingBox() const;
};

UCLASS()
class TETHER_API UTetherShapeObject_Capsule : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_Capsule; }

	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const override;

	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
