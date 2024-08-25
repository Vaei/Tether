// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "Shapes/TetherShape.h"

#include "TetherGameplayTags.h"

UTetherShapeObject* FTetherShape::GetTetherShape() const
{
	return TetherShapeClass ? TetherShapeClass.GetDefaultObject() : nullptr;
}

FGameplayTag FTetherShape::GetShapeType() const
{
	return GetTetherShape() ? GetTetherShape()->GetShapeType() : FGameplayTag::EmptyTag;
}

FVector FTetherShape::GetCenter() const
{
	return GetTetherShape() ? GetTetherShape()->GetLocalSpaceShapeCenter(*this) : FVector::ZeroVector;
}

bool FTetherShape::IsValid() const
{
	return GetTetherShape()
	&& GetShapeType().MatchesTag(FTetherGameplayTags::Tether_Shape)			// Starts with Tether.Shape
	&& !GetShapeType().MatchesTagExact(FTetherGameplayTags::Tether_Shape);	// But not actually Tether.Shape
}

bool FTetherShape::IsIgnored(const FTetherShape& Other) const
{
	if (!IsValid() || !Other.IsValid())
	{
		return true;
	}
	return IgnoredShapeTypes.HasTagExact(Other.GetShapeType()) || IgnoredShapes.Contains(Other.GetTetherShape());
}

bool FTetherShape::AreShapesIgnoringEachOther(const FTetherShape& ShapeA, const FTetherShape& ShapeB)
{
	return ShapeA.IsIgnored(ShapeB) || ShapeB.IsIgnored(ShapeA);
}

void FTetherShape::DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines,
	float LifeTime, float Thickness) const
{
	GetTetherShape()->DrawDebug(*this, AnimInstanceProxy, nullptr, Color, bPersistentLines, LifeTime, Thickness);
}

void FTetherShape::DrawDebug(UWorld* World, const FColor& Color, bool bPersistentLines, float LifeTime,
	float Thickness) const
{
	GetTetherShape()->DrawDebug(*this, nullptr, World, Color, bPersistentLines, LifeTime, Thickness);
}

void FTetherShape::ToWorldSpace(const FTransform& InWorldTransform)
{
	GetTetherShape()->TransformToWorldSpace(*this, InWorldTransform);
	bInWorldSpace = true;
	WorldTransform = InWorldTransform;
}

void FTetherShape::ToLocalSpace()
{
	GetTetherShape()->TransformToLocalSpace(*this);
	bInWorldSpace = false;
}
