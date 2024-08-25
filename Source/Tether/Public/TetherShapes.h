// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "TetherShapes.generated.h"

struct FTetherNarrowPhaseCollisionOutput;
struct FTetherShape_Cone;
struct FTetherShape_Capsule;
struct FTetherShape_OrientedBoundingBox;
struct FTetherShape_BoundingSphere;
struct FTetherShape_AxisAlignedBoundingBox;

UENUM(BlueprintType)
enum class ETetherShapeType : uint8
{
	Invalid				UMETA(Hidden),
	AABB				UMETA(DisplayName = "Axis-Aligned Bounding Box", ToolTip="AABBs are simpler to compute, as they only require finding the minimum and maximum extents of the object along the coordinate axes. Because they are axis-aligned, checking for overlaps between two AABBs is computationally cheap"),
	OBB					UMETA(DisplayName = "Oriented Bounding Box", ToolTip="Checking for overlaps between two OBBs is more computationally expensive than AABB, often requiring separating axis tests or other geometric methods."),
	BoundingSphere		UMETA(DisplayName = "Bounding Sphere", ToolTip="The Bounding Sphere is extremely simple to compute. It is defined by a center point and a radius, which can be derived from the furthest point from the center of the object."),
	Capsule				UMETA(DisplayName = "Capsule", ToolTip="Capsules are more complex than spheres due to their elongated shape, but they are simpler than boxes (OBB) when it comes to collision detection. Collision detection for capsules typically involves checking both the cylindrical part and the hemispherical ends, which is more complex than sphere collision detection but still relatively efficient."),
	Cone				UMETA(DisplayName = "Cone", ToolTip="The cone is more complex than both the sphere and capsule because of its narrowing shape and the need to handle the vertex (tip) in collision detection. Detecting collisions with a cone involves understanding its varying radius along the height and considering its angular properties, making it computationally more demanding."),
};

USTRUCT(BlueprintType)
struct TETHER_API FTetherShape
{
	GENERATED_BODY()

	FTetherShape()
	{
		// Generate a unique identifier for each shape
		Guid = FGuid::NewGuid();
	}
	
	virtual ~FTetherShape() = default;

	/** Unique identifier for the shape */
	UPROPERTY(BlueprintReadOnly, Category=Tether)
	FGuid Guid;

	/** List of shape IDs to ignore during collision checks */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	TArray<FGuid> IgnoreShapeIDs;

	/** List of shape types to ignore during collision checks */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	TArray<ETetherShapeType> IgnoreShapeTypes;

	/** Type of the shape */
	virtual ETetherShapeType GetShapeType() const { return ETetherShapeType::Invalid; }
	static ETetherShapeType StaticShapeType() { return ETetherShapeType::Invalid; }

	/** We can't check ShapeType is valid here because virtual doesn't function as we'd hope with derived struct types */
	bool IsValid() const { return GetShapeType() != ETetherShapeType::Invalid && Guid.IsValid(); }

	bool IsIgnored(const FTetherShape& Other) const
	{
		return IgnoreShapeTypes.Contains(Other.GetShapeType()) || IgnoreShapeIDs.Contains(Other.Guid);
	}

	/** Checks if two shapes are ignoring each other */
	static bool AreShapesIgnoringEachOther(const FTetherShape& ShapeA, const FTetherShape& ShapeB)
	{
		return ShapeA.IsIgnored(ShapeB) || ShapeB.IsIgnored(ShapeA);
	}

	virtual void ApplyTransform(const FTransform& Transform) { checkNoEntry(); }
	virtual FVector GetCenter() const { checkNoEntry(); return FVector::ZeroVector; }

	/** Checks if this shape is overlapping another shape */
	virtual bool CheckOverlap(const FTetherShape& Other) const { checkNoEntry(); return false; }

	/** Checks the detailed Narrow-Phase collisions against another shape */
	virtual bool CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const { checkNoEntry(); return false; }
	
	// Virtual functions to check overlap with specific shapes, to be overridden by derived classes
	virtual bool OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& Other) const { checkNoEntry(); return false; }
	virtual bool OverlapWithSphere(const FTetherShape_BoundingSphere& Other) const { checkNoEntry(); return false; }
	virtual bool OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const { checkNoEntry(); return false; }
	virtual bool OverlapWithCapsule(const FTetherShape_Capsule& Other) const { checkNoEntry(); return false;  }
	virtual bool OverlapWithCone(const FTetherShape_Cone& Other) const { checkNoEntry(); return false; }

	// Pure virtual function to draw debug shapes
	virtual void DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f) const { checkNoEntry(); }
	
	static FVector ClampVector(const FVector& InVector, const FVector& Min, const FVector& Max)
	{
		FVector Result;
		Result.X = FMath::Clamp(InVector.X, Min.X, Max.X);
		Result.Y = FMath::Clamp(InVector.Y, Min.Y, Max.Y);
		Result.Z = FMath::Clamp(InVector.Z, Min.Z, Max.Z);
		return Result;
	}

	bool operator==(const FTetherShape& Other) const
	{
		return IsValid() && Guid == Other.Guid;
	}
	
	bool operator!=(const FTetherShape& Other) const
	{
		return !(*this == Other);
	}
};

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
};

/**
 * AABBs are simpler to compute, as they only require finding the minimum and maximum extents of the object along the
 * coordinate axes.
 *
 * Because they are axis-aligned, checking for overlaps between two AABBs is computationally cheap
 */
USTRUCT(BlueprintType)
struct TETHER_API FTetherShape_AxisAlignedBoundingBox : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_AxisAlignedBoundingBox()
		: Min(FVector::ZeroVector)
		, Max(FVector::ZeroVector)
	{}

	FTetherShape_AxisAlignedBoundingBox(const FVector& InMin, const FVector& InMax)
		: Min(InMin)
		, Max(InMax)
	{}

	/** Minimum corner of the AABB */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Min;

	/** Maximum corner of the AABB */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Max;

	virtual ETetherShapeType GetShapeType() const override { return ETetherShapeType::AABB; }
	static ETetherShapeType StaticShapeType() { return ETetherShapeType::AABB; }

	virtual void ApplyTransform(const FTransform& Transform) override
	{
		// Transform both the min and max points
		FVector TransformedMin = Transform.TransformPosition(Min);
		FVector TransformedMax = Transform.TransformPosition(Max);

		// Ensure the transformed min and max are correctly aligned
		Min = FVector(FMath::Min(TransformedMin.X, TransformedMax.X), 
					  FMath::Min(TransformedMin.Y, TransformedMax.Y), 
					  FMath::Min(TransformedMin.Z, TransformedMax.Z));

		Max = FVector(FMath::Max(TransformedMin.X, TransformedMax.X), 
					  FMath::Max(TransformedMin.Y, TransformedMax.Y), 
					  FMath::Max(TransformedMin.Z, TransformedMax.Z));
	}
	
	virtual FVector GetCenter() const override
	{
		return (Min + Max) * 0.5f;
	}

	virtual bool CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const override;
	
	// Function to check if this AABB overlaps with any shape
	virtual bool CheckOverlap(const FTetherShape& Other) const override
	{
		return Other.OverlapWithAABB(*this);
	}

	// Function to check if this AABB overlaps with another AABB
	virtual bool OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& Other) const override
	{
		return AABBsOverlap(*this, Other);
	}

	static bool AABBsOverlap(const FTetherShape_AxisAlignedBoundingBox& A, const FTetherShape_AxisAlignedBoundingBox& B)
	{
		return (A.Min.X <= B.Max.X && A.Max.X >= B.Min.X) &&
			   (A.Min.Y <= B.Max.Y && A.Max.Y >= B.Min.Y) &&
			   (A.Min.Z <= B.Max.Z && A.Max.Z >= B.Min.Z);
	}

	virtual bool OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const override;
	virtual bool OverlapWithSphere(const FTetherShape_BoundingSphere& Sphere) const override;
	virtual bool OverlapWithCapsule(const FTetherShape_Capsule& Other) const override;
	virtual bool OverlapWithCone(const FTetherShape_Cone& Cone) const override;

	virtual void DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};

/**
 * Checking for overlaps between two OBBs is more computationally expensive than AABB,
 * often requiring separating axis tests or other geometric methods.
 */
USTRUCT(BlueprintType)
struct TETHER_API FTetherShape_OrientedBoundingBox : public FTetherShape
{
    GENERATED_BODY()

    FTetherShape_OrientedBoundingBox()
        : Center(FVector::ZeroVector)
        , Extent(FVector::OneVector)
        , Rotation(FRotator::ZeroRotator)
    {}

    FTetherShape_OrientedBoundingBox(const FVector& InCenter, const FVector& InExtent, const FRotator& InRotation)
        : Center(InCenter)
        , Extent(InExtent)
        , Rotation(InRotation)
    {}

    /** Center of the OBB */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
    FVector Center;

    /** Extents (half-sizes) of the OBB in each direction */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
    FVector Extent;

    /** Rotation of the OBB relative to the world or parent object */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
    FRotator Rotation;
	
	virtual ETetherShapeType GetShapeType() const override { return ETetherShapeType::OBB; }
	static ETetherShapeType StaticShapeType() { return ETetherShapeType::OBB; }

    virtual void ApplyTransform(const FTransform& Transform) override
    {
		// Update the center of the OBB with the actor's world position
		Center = Transform.TransformPosition(Center);

		// Update the rotation of the OBB
		Rotation = (Transform.GetRotation() * Rotation.Quaternion()).Rotator();

		// Optionally, scale the extents based on the actor's scale
		Extent *= Transform.GetScale3D();
	}

	virtual FVector GetCenter() const override
	{
		return Center;
	}

	virtual bool CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const override;

    // Function to check if this OBB overlaps with any shape
    virtual bool CheckOverlap(const FTetherShape& Other) const override
    {
        return Other.OverlapWithOBB(*this);
    }

    // Function to check if this OBB overlaps with another OBB
    virtual bool OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const override
    {
        return OBBsOverlap(*this, Other);
    }
	
	static bool OBBsOverlap(const FTetherShape_OrientedBoundingBox& A, const FTetherShape_OrientedBoundingBox& B)
    {
    	FTetherShape_AxisAlignedBoundingBox A_AABB = GenerateAABB(A);
    	FTetherShape_AxisAlignedBoundingBox B_AABB = GenerateAABB(B);
    	return FTetherShape_AxisAlignedBoundingBox::AABBsOverlap(A_AABB, B_AABB);
    }

	virtual bool OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& Other) const override;
	virtual bool OverlapWithSphere(const FTetherShape_BoundingSphere& Sphere) const override;
	virtual bool OverlapWithCapsule(const FTetherShape_Capsule& Other) const override;
	virtual bool OverlapWithCone(const FTetherShape_Cone& Cone) const override;
	
    // Generate an AABB from the OBB for simplified broad-phase collision detection
    static FTetherShape_AxisAlignedBoundingBox GenerateAABB(const FTetherShape_OrientedBoundingBox& OBB)
    {
        TArray<FVector> Vertices = OBB.GetVertices();
        FVector Min = Vertices[0];
        FVector Max = Vertices[0];

        for (const FVector& Vertex : Vertices)
        {
            Min = Min.ComponentMin(Vertex);
            Max = Max.ComponentMax(Vertex);
        }

        return FTetherShape_AxisAlignedBoundingBox(Min, Max);
    }

    TArray<FVector> GetVertices() const
    {
        const FQuat QuatRotation = Rotation.Quaternion();
        TArray<FVector> Vertices;
        FVector Axes[3] =
        {
            QuatRotation.GetAxisX() * Extent.X,
            QuatRotation.GetAxisY() * Extent.Y,
            QuatRotation.GetAxisZ() * Extent.Z
        };

        for (int32 i = -1; i <= 1; i += 2)
        {
            for (int32 j = -1; j <= 1; j += 2)
            {
                for (int32 k = -1; k <= 1; k += 2)
                {
                    Vertices.Add(Center + i * Axes[0] + j * Axes[1] + k * Axes[2]);
                }
            }
        }

        return Vertices;
    }

	virtual void DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};

/**
 * The Bounding Sphere is extremely simple to compute. It is defined by a center point and a radius, which can be
 * derived from the furthest point from the center of the object.
 *
 * Collision detection between spheres (and between a sphere and other shapes) is very straightforward,
 * involving basic distance checks.
 */
USTRUCT(BlueprintType)
struct TETHER_API FTetherShape_BoundingSphere : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_BoundingSphere()
		: Center(FVector::ZeroVector)
		, Radius(30.f)
	{}

	FTetherShape_BoundingSphere(const FVector& InCenter, float InRadius)
		: Center(InCenter)
		, Radius(InRadius)
	{}

	/** Center of the sphere */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Center;

	/** Radius of the sphere */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float Radius;

	virtual ETetherShapeType GetShapeType() const override { return ETetherShapeType::BoundingSphere; }
	static ETetherShapeType StaticShapeType() { return ETetherShapeType::BoundingSphere; }

	virtual void ApplyTransform(const FTransform& Transform) override
	{
		// Update the center of the sphere with the actor's world position
		Center = Transform.TransformPosition(Center);
	}
	
	virtual FVector GetCenter() const override
	{
		return Center;
	}

	virtual bool CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const override;
	
	// Function to check if this sphere overlaps with any shape
	virtual bool CheckOverlap(const FTetherShape& Other) const override
	{
		return Other.OverlapWithSphere(*this);
	}

	// Function to check if this sphere overlaps with another sphere
	virtual bool OverlapWithSphere(const FTetherShape_BoundingSphere& Other) const override
	{
		return SpheresOverlap(*this, Other);
	}

	virtual bool OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& AABB) const override;
	virtual bool OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const override;
	virtual bool OverlapWithCapsule(const FTetherShape_Capsule& Other) const override;
	virtual bool OverlapWithCone(const FTetherShape_Cone& Cone) const override;
	
	static bool SpheresOverlap(const FTetherShape_BoundingSphere& A, const FTetherShape_BoundingSphere& B)
	{
		float DistanceSquared = FVector::DistSquared(A.Center, B.Center);
		float RadiusSum = A.Radius + B.Radius;
		return DistanceSquared <= RadiusSum * RadiusSum;
	}

	virtual void DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};

/**
 * Capsules are more complex than spheres due to their elongated shape, but they are simpler than boxes (OBB) when it
 * comes to collision detection.
 *
 * Collision detection for capsules typically involves checking both the cylindrical part and the hemispherical ends,
 * which is more complex than sphere collision detection but still relatively efficient.
 */
USTRUCT(BlueprintType)
struct TETHER_API FTetherShape_Capsule : public FTetherShape
{
    GENERATED_BODY()

    FTetherShape_Capsule()
        : Center(FVector::ZeroVector)
        , HalfHeight(50.0f)
        , Radius(25.0f)
        , Rotation(FRotator::ZeroRotator)
    {}

    FTetherShape_Capsule(const FVector& InCenter, float InHalfHeight, float InRadius, const FRotator& InRotation)
        : Center(InCenter)
        , HalfHeight(InHalfHeight)
        , Radius(InRadius)
        , Rotation(InRotation)
    {}

    /** Center of the capsule */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
    FVector Center;

    /** Half-height of the capsule (from the center to one of the hemispherical ends) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
    float HalfHeight;

    /** Radius of the capsule */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
    float Radius;

    /** Rotation of the capsule */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
    FRotator Rotation;
	
	virtual ETetherShapeType GetShapeType() const override { return ETetherShapeType::Capsule; }
	static ETetherShapeType StaticShapeType() { return ETetherShapeType::Capsule; }

	virtual void ApplyTransform(const FTransform& Transform) override
	{
		// Update the center of the capsule with the actor's world position
		Center = Transform.TransformPosition(Center);

		// Update the rotation of the capsule
		Rotation = (Transform.GetRotation() * Rotation.Quaternion()).Rotator();

		// Scale the radius and half-height based on the actor's scale
		FVector Scale = Transform.GetScale3D();
		Radius *= FMath::Max(Scale.X, Scale.Y); // Assuming uniform scaling in X and Y for radius
		HalfHeight *= Scale.Z; // Assuming Z scale for the height
	}
	
	virtual FVector GetCenter() const override
	{
		FVector Top, Bottom;
		GetCapsulePoints(Top, Bottom);

		// The center of the capsule is the midpoint between the top and bottom points
		return (Top + Bottom) * 0.5f;
	}

	virtual bool CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const override;

    // Function to check if this capsule overlaps with any shape
    virtual bool CheckOverlap(const FTetherShape& Other) const override
    {
        return Other.OverlapWithCapsule(*this);
    }

    // Function to check if this capsule overlaps with another capsule
    virtual bool OverlapWithCapsule(const FTetherShape_Capsule& Other) const override
    {
        return CapsulesOverlap(*this, Other);
    }

	static bool CapsulesOverlap(const FTetherShape_Capsule& A, const FTetherShape_Capsule& B)
    {
    	FTetherShape_BoundingSphere SphereA = A.GenerateBoundingSphere();
    	FTetherShape_BoundingSphere SphereB = B.GenerateBoundingSphere();
    	return FTetherShape_BoundingSphere::SpheresOverlap(SphereA, SphereB);
    }

	virtual bool OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& Other) const override;
	virtual bool OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const override;
    virtual bool OverlapWithSphere(const FTetherShape_BoundingSphere& Sphere) const override;
    virtual bool OverlapWithCone(const FTetherShape_Cone& Cone) const override;

    FTetherShape_BoundingSphere GenerateBoundingSphere() const
    {
        return FTetherShape_BoundingSphere(Center, HalfHeight + Radius);
    }

    void GetCapsulePoints(FVector& OutTop, FVector& OutBottom) const
    {
        FVector Up = Rotation.Quaternion().GetAxisZ() * HalfHeight;
        OutTop = Center + Up;
        OutBottom = Center - Up;
    }

	virtual void DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines, float LifeTime, float Thickness) const override;
};

/**
 * The cone is more complex than both the sphere and capsule because of its narrowing shape and the need to handle the
 * vertex (tip) in collision detection.
 *
 * Detecting collisions with a cone involves understanding its varying radius along the height and considering its
 * angular properties, making it computationally more demanding.
 */
USTRUCT(BlueprintType)
struct TETHER_API FTetherShape_Cone : public FTetherShape
{
	GENERATED_BODY()

	FTetherShape_Cone()
		: BaseCenter(FVector::ZeroVector)
		, Height(100.0f)
		, BaseRadius(50.0f)
		, Rotation(FRotator::ZeroRotator)
	{}

	FTetherShape_Cone(const FVector& InBaseCenter, float InHeight, float InRadius, const FRotator& InRotation)
		: BaseCenter(InBaseCenter)
		, Height(InHeight)
		, BaseRadius(InRadius)
		, Rotation(InRotation)
	{}

	/** Center of the cone's base */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector BaseCenter;

	/** Height of the cone from base to the tip */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float Height;

	/** Radius of the cone's base */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float BaseRadius;

	/** Rotation of the cone, defining its orientation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FRotator Rotation;
	
	virtual ETetherShapeType GetShapeType() const override { return ETetherShapeType::Cone; }
	static ETetherShapeType StaticShapeType() { return ETetherShapeType::Cone; }

	virtual void ApplyTransform(const FTransform& Transform) override
	{
		// Update the base center of the cone with the actor's world position
		BaseCenter = Transform.TransformPosition(BaseCenter);

		// Update the rotation of the cone
		Rotation = (Transform.GetRotation() * Rotation.Quaternion()).Rotator();

		// Scale the base radius and height based on the actor's scale
		FVector Scale = Transform.GetScale3D();
		BaseRadius *= FMath::Max(Scale.X, Scale.Y); // Assuming uniform scaling in X and Y for the base radius
		Height *= Scale.Z; // Assuming Z scale for the height
	}
	
	virtual FVector GetCenter() const override
	{
		// Calculate the tip of the cone based on its height and rotation
		FVector Tip = BaseCenter + Rotation.RotateVector(FVector(0.0f, 0.0f, Height));

		// The center of the cone is the midpoint between the base center and the tip
		return (BaseCenter + Tip) * 0.5f;
	}

	virtual bool CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const override;

	/** Pure virtual function to check overlap with any shape */
	virtual bool CheckOverlap(const FTetherShape& Other) const override
	{
		return Other.OverlapWithCone(*this);
	}

	virtual bool OverlapWithCone(const FTetherShape_Cone& Other) const override
	{
		// A simplified approach: Check if the tips are within each other's volume
		FVector ToOtherTip = Other.BaseCenter + Other.Rotation.RotateVector(FVector(0.0f, 0.0f, Other.Height)) - BaseCenter;
		FVector ToThisTip = BaseCenter + Rotation.RotateVector(FVector(0.0f, 0.0f, Height)) - Other.BaseCenter;

		FVector ThisDirection = Rotation.RotateVector(FVector::UpVector);
		FVector OtherDirection = Other.Rotation.RotateVector(FVector::UpVector);

		// Check if the tip of the other cone is within this cone's volume
		float HeightAlongThisCone = FVector::DotProduct(ToOtherTip, ThisDirection);
		float RadiusAtThisHeight = (HeightAlongThisCone / Height) * BaseRadius;

		bool OtherTipInThisCone = HeightAlongThisCone >= 0.0f && HeightAlongThisCone <= Height && ToOtherTip.SizeSquared() <= FMath::Square(RadiusAtThisHeight);

		// Check if the tip of this cone is within the other cone's volume
		float HeightAlongOtherCone = FVector::DotProduct(ToThisTip, OtherDirection);
		float RadiusAtOtherHeight = (HeightAlongOtherCone / Other.Height) * Other.BaseRadius;

		bool ThisTipInOtherCone = HeightAlongOtherCone >= 0.0f && HeightAlongOtherCone <= Other.Height && ToThisTip.SizeSquared() <= FMath::Square(RadiusAtOtherHeight);

		return OtherTipInThisCone || ThisTipInOtherCone;
	}

	virtual bool OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& Other) const override;
	virtual bool OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const override;
	virtual bool OverlapWithSphere(const FTetherShape_BoundingSphere& Other) const override;
	virtual bool OverlapWithCapsule(const FTetherShape_Capsule& Other) const override;

	/** Pure virtual function to draw debug shapes */
	virtual void DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines = false, float LifeTime = -1.f, float Thickness = 0.f) const override;
};