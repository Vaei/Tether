// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherShape.h"

/**
 * Cast<>() & IsA<>() equivalent for FTetherShape types
 */
struct TETHER_API FTetherShapeTypeCaster
{
	/** Check if the shape is of the specified type */
	template <typename ShapeType>
	static bool IsShapeChildOf(const FTetherShape* InShape)
	{
		if (InShape)
		{
			return InShape->GetShapeType() == ShapeType::StaticShapeType();
		}
		return false;
	}

	/** Cast the shape to the specified type */
	template <typename ShapeType>
	static const ShapeType* CastShape(const FTetherShape* InShape)
	{
		if (IsShapeChildOf<ShapeType>(InShape))
		{
			return static_cast<const ShapeType*>(InShape);
		}
		return nullptr;
	}

	/** Cast the shape to the specified type - engine crashes if wrong type is given */
	template <typename ShapeType>
	static const ShapeType* CastShapeChecked(const FTetherShape* InShape)
	{
		return static_cast<const ShapeType*>(InShape);
	}

	/** Cast the shape to the specified type */
	template <typename ShapeType>
	static ShapeType* CastShape(FTetherShape* InShape)
	{
		if (IsShapeChildOf<ShapeType>(InShape))
		{
			return static_cast<ShapeType*>(InShape);
		}
		return nullptr;
	}

	/** Cast the shape to the specified type - engine crashes if wrong type is given */
	template <typename ShapeType>
	static ShapeType* CastShapeChecked(FTetherShape* InShape)
	{
		return static_cast<ShapeType*>(InShape);
	}
	
	/** Cast the shape to the specified type */
	template <typename ShapeType>
	static ShapeType& CastShape(const FTetherShape& InShape)
	{
		if (IsShapeChildOf<ShapeType>(InShape))
		{
			return static_cast<const ShapeType&>(InShape);
		}
		return nullptr;
	}

	/** Cast the shape to the specified type - engine crashes if wrong type is given */
	template <typename ShapeType>
	static ShapeType& CastShapeChecked(const FTetherShape& InShape)
	{
		return static_cast<const ShapeType&>(InShape);
	}

	/** Cast the shape to the specified type */
	template <typename ShapeType>
	static ShapeType& CastShape(FTetherShape& InShape)
	{
		if (IsShapeChildOf<ShapeType>(InShape))
		{
			return static_cast<ShapeType&>(InShape);
		}
		return nullptr;
	}

	/** Cast the shape to the specified type - engine crashes if wrong type is given */
	template <typename ShapeType>
	static ShapeType& CastShapeChecked(FTetherShape& InShape)
	{
		return static_cast<ShapeType&>(InShape);
	}
};