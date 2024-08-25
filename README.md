# Tether

***IN ACTIVE DEVELOPMENT AND NOT FUNCTIONAL - YET***
_DO NOT EXPECT THIS TO COMPILE OR FUNCTION AT ALL_

Modular framework for integrating physics solvers and constraints into animation graph.

Ideal for simulating objects attached to characters, such as accessories or equipment, with customizable and extendable options for realistic, physics-driven animations.

# Instructions
_TODO_

# Guides
_WIP_

## Add Custom Shapes
_WIP - THESE ARE JUST NOTES FOR NOW_

_Adding custom shapes is an exceptionally mathematically advanced topic!_

*Note: For example implementations, simply look at the existing tether shapes! You can find these in Tether/Public/Shapes*

In this example, all we're really doing is reimplementing the BoundingSphere, but from scratch, in our own project, extending the plugin's modular shape framework.

Look at `TetherGameplayTags.h` and `.cpp`, you will need to add the same kind of setup to your own project, then add a tag, e.g. `Tether.Shape.MyNewShape`; it _must_ begin with `Tether.Shape`. Don't forget to add "GameplayTags" to your Build.cs

Subclass `UTetherShapeObject`, usually as `UTetherShapeObject_MyNewShape`
Within `UTetherShapeObject_MyNewShape`, create a struct matching the template below:

```cpp
USTRUCT(BlueprintType)
struct FTetherShape_MyNewShape : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_MyNewShape();

	FTetherShape_MyNewShape(const FVector& InMin, const FVector& InMax);

	virtual TSharedPtr<FTetherShape> Clone() const override { return MakeShared<FTetherShape_MyNewShape>(*this); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float ShapeSize;  // EXAMPLE

	static FGameplayTag StaticShapeType() { return FMyGameplayTags::Tether_Shape_MyNewShape; }
};
```

You must be sure to assign the `TetherShapeClass` in the ctor. Expect engine crashes if you miss this step.
```cpp
FTetherShape_MyNewShape::FTetherShape_MyNewShape()
	: ShapeSize(30.f)
{
	TetherShapeClass = UTetherShapeObject_MyNewShape::StaticClass();
}

FTetherShape_MyNewShape::FTetherShape_MyNewShape(float InShapeSize)
	: ShapeSize(InShapeSize)
{
	TetherShapeClass = UTetherShapeObject_MyNewShape::StaticClass();
}
```

Override all functions from `UTetherShapeObject`:
```cpp
	virtual FGameplayTag GetShapeType() const override { return FMyGameplayTags::Tether_Shape_MyNewShape;	}

	virtual FVector GetLocalSpaceShapeCenter(const FTetherShape& Shape) const override;
	
	virtual void TransformToWorldSpace(FTetherShape& Shape, const FTransform& WorldTransform) const override;
	virtual void TransformToLocalSpace(FTetherShape& Shape) const override;

	virtual void DrawDebug(const FTetherShape& Shape, FAnimInstanceProxy* AnimInstanceProxy, UWorld* World,
		const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
```

Example implementation:
```cpp

```

Subclass `UTetherShapeCollisionControl`


# Changelog

### 1.0.0
* Initial Release
