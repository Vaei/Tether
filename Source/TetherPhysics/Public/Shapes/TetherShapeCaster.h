// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherShape.h"

/**
 * Utility class for casting and type-checking FTetherShape types.
 *
 * FTetherShapeCaster provides a set of static methods that mimic the behavior of Unreal Engine's
 * `Cast<>()` and `IsA<>()` functions but are designed specifically for use with FTetherShape types.
 * These methods allow you to safely check the type of a shape and cast it to a specific derived type.
 *
 * Key Features:
 * - IsChildOf: Checks whether a given shape is of the specified type.
 * - Cast: Safely casts a shape to a specified type if it is of that type, returning nullptr if not.
 * - CastChecked: Forcefully casts a shape to a specified type, causing an engine crash if the cast is invalid.
 *
 * These functions are particularly useful in scenarios where polymorphism is manually managed within the
 * Tether physics system, allowing for safe type-checking and casting of shapes without relying on virtual methods.
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

	/** Cast the shape to the specified type (const version) */
	template <typename ShapeType>
	static const ShapeType* Cast(const FTetherShape* InShape)
	{
		if (IsChildOf<ShapeType>(InShape))
		{
			return static_cast<const ShapeType*>(InShape);
		}
		return nullptr;
	}

	/** Forcefully cast the shape to the specified type (const version) - engine crashes if wrong type is given */
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

	/** Forcefully cast the shape to the specified type - engine crashes if wrong type is given */
	template <typename ShapeType>
	static ShapeType* CastChecked(FTetherShape* InShape)
	{
		return static_cast<ShapeType*>(InShape);
	}
	
	/** Cast the shape to the specified type (const reference version) */
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

	/** Forcefully cast the shape to the specified type (const reference version) - engine crashes if wrong type is given */
	template <typename ShapeType>
	static ShapeType& Cast(FTetherShape& InShape)
	{
		if (IsChildOf<ShapeType>(InShape))
		{
			return static_cast<ShapeType&>(InShape);
		}
		return nullptr;
	}

	/** Forcefully cast the shape to the specified type (reference version) - engine crashes if wrong type is given */
	template <typename ShapeType>
	static ShapeType& CastChecked(FTetherShape& InShape)
	{
		return static_cast<ShapeType&>(InShape);
	}
};