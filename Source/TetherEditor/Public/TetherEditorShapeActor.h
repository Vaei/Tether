// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"
#include "Shapes/TetherShape_BoundingSphere.h"
#include "Shapes/TetherShape_Capsule.h"
#include "Shapes/TetherShape_OrientedBoundingBox.h"
#include "TetherEditorShapeActor.generated.h"

/**
 * This class exists for the purpose of testing FTetherShape behaviour and collisions
 */
UCLASS(Blueprintable)
class TETHEREDITOR_API ATetherEditorShapeActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(Categories="Tether.Shape"))
	FGameplayTag ShapeType = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="Axis-Aligned Bounding Box"))
	FTetherShape_AxisAlignedBoundingBox AABB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="Oriented Bounding Box"))
	FTetherShape_OrientedBoundingBox OBB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="Bounding Sphere"))
	FTetherShape_BoundingSphere BoundingSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="Capsule"))
	FTetherShape_Capsule Capsule;

public:
	ATetherEditorShapeActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual const FTetherShape& GetTetherShape() const;
	
	void TestCollisions(const TArray<ATetherEditorShapeActor*>& OtherShapeActors) const;

protected:
	void TestCollision(const ATetherEditorShapeActor* OtherShapeActor) const;

public:
	virtual bool CanEditChange(const FProperty* InProperty) const override;
};
