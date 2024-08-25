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
