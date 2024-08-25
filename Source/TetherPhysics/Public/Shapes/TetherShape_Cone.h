// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherGameplayTags.h"
#include "TetherShape.h"
#include "TetherShape_AxisAlignedBoundingBox.h"
#include "TetherShape_Cone.generated.h"

USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherShape_Cone : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_Cone();

	FTetherShape_Cone(const FVector& InBaseCenter, float InHeight, float InBaseRadius, const FRotator& InRotation);

	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_Cone>(*this); }

	static FGameplayTag StaticShapeType() { return FTetherGameplayTags::Tether_Shape_Cone; }

	/** Base center of the cone */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector BaseCenter;

	/** Height of the cone */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float Height;

	/** Radius of the cone's base */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(ClampMin="0", UIMin="0"))
	float BaseRadius;

	/** Rotation of the cone */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FRotator Rotation;

	FTetherShape_AxisAlignedBoundingBox GetBoundingBox() const;
};

UCLASS()
class TETHERPHYSICS_API UTetherShapeObject_Cone : public UTetherShapeObject
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetShapeType() const override { return FTetherGameplayTags::Tether_Shape_Cone; }

	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const override;

	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};
