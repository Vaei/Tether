﻿// Copyright (c) Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShape.h"

#include "TetherGameplayTags.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShape)

UTetherShapeObject* FTetherShape::GetTetherShapeObject() const
{
	return TetherShapeClass ? TetherShapeClass.GetDefaultObject() : nullptr;
}

FGameplayTag FTetherShape::GetShapeType() const
{
	return GetTetherShapeObject() ? GetTetherShapeObject()->GetShapeType() : FGameplayTag::EmptyTag;
}

FString FTetherShape::GetName() const
{
	return GetTetherShapeObject() ? GetTetherShapeObject()->GetShapeDebugString() : GetShapeType().ToString();
}

FVector FTetherShape::GetLocalSpaceCenter() const
{
	return GetTetherShapeObject() ? GetTetherShapeObject()->GetLocalSpaceShapeCenter(*this) : FVector::ZeroVector;
}

bool FTetherShape::IsValid() const
{
	return GetTetherShapeObject()
	&& GetShapeType().MatchesTag(FTetherGameplayTags::Tether_Shape)			// Starts with Tether.Shape
	&& !GetShapeType().MatchesTagExact(FTetherGameplayTags::Tether_Shape);	// But not actually Tether.Shape
}

bool FTetherShape::IsIgnored(const FTetherShape& Other) const
{
	if (!IsValid() || !Other.IsValid())
	{
		return true;
	}
	return IgnoredShapeTypes.HasTagExact(Other.GetShapeType()) || IgnoredShapes.Contains(Other.GetTetherShapeObject());
}

bool FTetherShape::AreShapesIgnoringEachOther(const FTetherShape& ShapeA, const FTetherShape& ShapeB)
{
	return ShapeA.IsIgnored(ShapeB) || ShapeB.IsIgnored(ShapeA);
}

void FTetherShape::ToWorldSpace(const FTransform& InWorldTransform)
{
	GetTetherShapeObject()->TransformToWorldSpace(*this, InWorldTransform);
	bWorldSpace = true;
	AppliedWorldTransform = InWorldTransform;
}

void FTetherShape::ToLocalSpace()
{
	GetTetherShapeObject()->TransformToLocalSpace(*this);
	bWorldSpace = false;
}

void FTetherShape::DrawDebug(const UWorld* World, FAnimInstanceProxy* Proxy, const FColor& Color,
	bool bPersistentLines, float LifeTime, float Thickness) const
{
#if ENABLE_DRAW_DEBUG
	GetTetherShapeObject()->DrawDebug(*this, Proxy, World, Color, bPersistentLines, LifeTime, Thickness);
#endif
}

FTetherShape_AxisAlignedBoundingBox UTetherShapeObject::GetBoundingBox(const FTetherShape& Shape) const
{
	return FTetherShape_AxisAlignedBoundingBox();
}
