// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="AxisAlignedBoundingBox"))
	FTetherShape_AxisAlignedBoundingBox AABB;

	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="OrientedBoundingBox", EditCondition="ShapeType==ETetherShapeType::OBB", EditConditionHides))
	// FTetherShape_OrientedBoundingBox OBB;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="BoundingSphere", EditCondition="ShapeType==ETetherShapeType::BoundingSphere", EditConditionHides))
	// FTetherShape_BoundingSphere BoundingSphere;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="Capsule", EditCondition="ShapeType==ETetherShapeType::Capsule", EditConditionHides))
	// FTetherShape_Capsule Capsule;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="Cone", EditCondition="ShapeType==ETetherShapeType::Cone", EditConditionHides))
	// FTetherShape_Cone Cone;
	//
public:
	ATetherEditorShapeActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual const FTetherShape& GetTetherShape() const;
	
	void TestCollisions(const TArray<ATetherEditorShapeActor*>& OtherShapeActors) const;

protected:
	void TestCollision(const ATetherEditorShapeActor* OtherShapeActor) const;

public:
	virtual bool CanEditChange(const FProperty* InProperty) const override;
};
