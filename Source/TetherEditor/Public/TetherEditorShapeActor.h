// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TetherPhysicsTypes.h"
#include "GameFramework/Actor.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"
#include "Shapes/TetherShape_OrientedBoundingBox.h"
#include "Shapes/TetherShape_BoundingSphere.h"
#include "Shapes/TetherShape_Capsule.h"
#include "Shapes/TetherShape_Pipe.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(DisplayName="Pipe"))
	FTetherShape_Pipe Pipe;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FTetherCommonShapeData ShapeData;

public:
	ATetherEditorShapeActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	virtual FTetherShape* GetTetherShape();

public:
	virtual bool CanEditChange(const FProperty* InProperty) const override;

};
