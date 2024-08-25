// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherShape.h"

/**
 * Cast<>() & IsA<>() equivalent for FTetherShape types
 */
struct TETHERPHYSICS_API FTetherShapeCaster
{
	/** Check if the shape is of the specified type */
	template <typename ShapeType>
	static bool IsChildOf(const FTetherShape* InShape)
	{
		if (InShape)
		{
			return InShape->GetShapeType() == ShapeType::StaticShapeType();
		}
		return false;
	}

	/** Cast the shape to the specified type */
	template <typename ShapeType>
	static const ShapeType* Cast(const FTetherShape* InShape)
	{
		if (IsChildOf<ShapeType>(InShape))
		{
			return static_cast<const ShapeType*>(InShape);
		}
		return nullptr;
	}

	/** Cast the shape to the specified type - engine crashes if wrong type is given */
	template <typename ShapeType>
	static const ShapeType* CastChecked(const FTetherShape* InShape)
	{
		return static_cast<const ShapeType*>(InShape);
	}

	/** Cast the shape to the specified type */
	template <typename ShapeType>
	static ShapeType* Cast(FTetherShape* InShape)
	{
		if (IsChildOf<ShapeType>(InShape))
		{
			return static_cast<ShapeType*>(InShape);
		}
		return nullptr;
	}

	/** Cast the shape to the specified type - engine crashes if wrong type is given */
	template <typename ShapeType>
	static ShapeType* CastChecked(FTetherShape* InShape)
	{
		return static_cast<ShapeType*>(InShape);
	}
	
	/** Cast the shape to the specified type */
	template <typename ShapeType>
	static ShapeType& Cast(const FTetherShape& InShape)
	{
		if (IsChildOf<ShapeType>(InShape))
		{
			return static_cast<const ShapeType&>(InShape);
		}
		return nullptr;
	}

	/** Cast the shape to the specified type - engine crashes if wrong type is given */
	template <typename ShapeType>
	static ShapeType& CastChecked(const FTetherShape& InShape)
	{
		return static_cast<const ShapeType&>(InShape);
	}

	/** Cast the shape to the specified type */
	template <typename ShapeType>
	static ShapeType& Cast(FTetherShape& InShape)
	{
		if (IsChildOf<ShapeType>(InShape))
		{
			return static_cast<ShapeType&>(InShape);
		}
		return nullptr;
	}

	/** Cast the shape to the specified type - engine crashes if wrong type is given */
	template <typename ShapeType>
	static ShapeType& CastChecked(FTetherShape& InShape)
	{
		return static_cast<ShapeType&>(InShape);
	}
};