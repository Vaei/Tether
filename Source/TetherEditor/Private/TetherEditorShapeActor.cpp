// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherEditorShapeActor.h"

#include "TetherSettings.h"
#include "TetherEditorSubsystem.h"
#include "Shapes/TetherShape.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherEditorShapeActor)

ATetherEditorShapeActor::ATetherEditorShapeActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Editor only actors can't be saved into a level, which is annoying for testing - need to be deleted before packaging!
	// bIsEditorOnlyActor = true;

	// These are ticked by TetherEditorSubsystem
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ATetherEditorShapeActor::BeginPlay()
{
	Super::BeginPlay();

	// Register with Subsystem
	if (auto* TetherEditorSubsystem = UTetherEditorSubsystem::Get(GetWorld()))
	{
		TetherEditorSubsystem->ShapeActors.Add(this);
	}
}

void ATetherEditorShapeActor::Destroyed()
{
	if (auto* TetherEditorSubsystem = UTetherEditorSubsystem::Get(GetWorld()))
	{
		TetherEditorSubsystem->ShapeActors.Remove(this);
	}
	Super::Destroyed();
}

FTetherShape* ATetherEditorShapeActor::GetTetherShape()
{
	if (ShapeType == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox) { return &AABB; }
	if (ShapeType == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox) { return &OBB; }
	if (ShapeType == FTetherGameplayTags::Tether_Shape_BoundingSphere) { return &BoundingSphere; }
	if (ShapeType == FTetherGameplayTags::Tether_Shape_Capsule) { return &Capsule; }
	if (ShapeType == FTetherGameplayTags::Tether_Shape_Pipe) { return &Pipe; }
	
	return &AABB;
}

bool ATetherEditorShapeActor::CanEditChange(const FProperty* InProperty) const
{
	if (InProperty->GetFName().IsEqual(GET_MEMBER_NAME_CHECKED(ThisClass, AABB)))
	{
		return ShapeType == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox;
	}
	if (InProperty->GetFName().IsEqual(GET_MEMBER_NAME_CHECKED(ThisClass, OBB)))
	{
		return ShapeType == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox;
	}
	if (InProperty->GetFName().IsEqual(GET_MEMBER_NAME_CHECKED(ThisClass, BoundingSphere)))
	{
		return ShapeType == FTetherGameplayTags::Tether_Shape_BoundingSphere;
	}
	if (InProperty->GetFName().IsEqual(GET_MEMBER_NAME_CHECKED(ThisClass, Capsule)))
	{
		return ShapeType == FTetherGameplayTags::Tether_Shape_Capsule;
	}
	return Super::CanEditChange(InProperty);
}
