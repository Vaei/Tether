// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherShapes.h"

#include "TetherIO.h"
#include "Animation/AnimInstanceProxy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherShapes)

bool FTetherShape_AxisAlignedBoundingBox::CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const
{
    // Check for collision with a Bounding Sphere
    if (const FTetherShape_BoundingSphere* OtherSphere = FTetherShapeTypeCaster::CastShape<FTetherShape_BoundingSphere>(&Other))
    {
        // Perform detailed AABB vs. Sphere collision detection
        FVector ClosestPoint = ClampVector(OtherSphere->Center, Min, Max);
        float DistanceSquared = FVector::DistSquared(ClosestPoint, OtherSphere->Center);

        if (DistanceSquared <= FMath::Square(OtherSphere->Radius))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = ClosestPoint;
            Output.PenetrationDepth = OtherSphere->Radius - FMath::Sqrt(DistanceSquared);
            return true;
        }
    }
    // Check for collision with an Oriented Bounding Box (OBB)
    else if (const FTetherShape_OrientedBoundingBox* OtherOBB = FTetherShapeTypeCaster::CastShape<FTetherShape_OrientedBoundingBox>(&Other))
    {
        if (OverlapWithOBB(*OtherOBB))
        {
            Output.bHasCollision = true;
            // Use the center of the OBB as the contact point (can be refined)
            Output.ContactPoint = OtherOBB->Center;
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with a Capsule
    else if (const FTetherShape_Capsule* OtherCapsule = FTetherShapeTypeCaster::CastShape<FTetherShape_Capsule>(&Other))
    {
        if (OverlapWithCapsule(*OtherCapsule))
        {
            Output.bHasCollision = true;
            // Use the closest point on the capsule as the contact point (can be refined)
            FVector CapsuleTop, CapsuleBottom;
            OtherCapsule->GetCapsulePoints(CapsuleTop, CapsuleBottom);
            FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(GetCenter(), CapsuleBottom, CapsuleTop);

            Output.ContactPoint = ClosestPointOnCapsule;
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with another AABB
    else if (const FTetherShape_AxisAlignedBoundingBox* OtherAABB = FTetherShapeTypeCaster::CastShape<FTetherShape_AxisAlignedBoundingBox>(&Other))
    {
        // Perform AABB vs. AABB overlap check
        if ((Min.X <= OtherAABB->Max.X && Max.X >= OtherAABB->Min.X) &&
            (Min.Y <= OtherAABB->Max.Y && Max.Y >= OtherAABB->Min.Y) &&
            (Min.Z <= OtherAABB->Max.Z && Max.Z >= OtherAABB->Min.Z))
        {
            Output.bHasCollision = true;
            // Use the midpoint between the two AABBs as a simple contact point
            Output.ContactPoint = (GetCenter() + OtherAABB->GetCenter()) * 0.5f;
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with a Cone
    else if (const FTetherShape_Cone* OtherCone = FTetherShapeTypeCaster::CastShape<FTetherShape_Cone>(&Other))
    {
        if (OtherCone->OverlapWithAABB(*this))
        {
            Output.bHasCollision = true;
            // Use the cone's base center as a simple contact point (can be refined)
            Output.ContactPoint = OtherCone->BaseCenter;
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }

    return false;
}

bool FTetherShape_AxisAlignedBoundingBox::OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const
{
	/*
	 * To check for overlap between an Axis-Aligned Bounding Box (AABB) and an Oriented Bounding Box (OBB),
	 * we can use the Separating Axis Theorem (SAT). This method checks whether there is a separating axis
	 * between the two shapes; if no such axis exists, the shapes overlap.
	 */
	
    // Step 1: Compute the half-extents of the AABB
    FVector AABBCenter = GetCenter();
    FVector AABBExtents = (Max - Min) * 0.5f;

    // Step 2: Get the OBB's axes and half-extents
    TArray<FVector> OBBVertices = Other.GetVertices();
    FVector OBBHalfExtents = Other.Extent;

    FQuat OBBRotation = Other.Rotation.Quaternion();

    FVector OBBX = OBBRotation.GetAxisX();
    FVector OBBY = OBBRotation.GetAxisY();
    FVector OBBZ = OBBRotation.GetAxisZ();

    FVector OBBPosition = Other.Center;

    // Step 3: Compute the distance vector between the centers
    FVector Distance = OBBPosition - AABBCenter;

    // Step 4: Check each axis (AABB's axes and OBB's axes)
    TArray<FVector> Axes = {
        FVector::ForwardVector, // AABB X-axis
        FVector::RightVector, // AABB Y-axis
        FVector::UpVector, // AABB Z-axis
        OBBX,
        OBBY,
        OBBZ
    };

    for (const FVector& Axis : Axes)
    {
        // Project the half-extents onto the axis
    	const FVector AbsAxis = Axis.GetAbs();
        float AABBProjection = FVector::DotProduct(AABBExtents * AbsAxis.X, FVector::ForwardVector) +
                               FVector::DotProduct(AABBExtents * AbsAxis.Y, FVector::RightVector) +
                               FVector::DotProduct(AABBExtents * AbsAxis.Z, FVector::UpVector);

        float OBBProjection = FVector::DotProduct(OBBX * OBBHalfExtents.X, Axis) +
                              FVector::DotProduct(OBBY * OBBHalfExtents.Y, Axis) +
                              FVector::DotProduct(OBBZ * OBBHalfExtents.Z, Axis);

        float DistanceProjection = FMath::Abs(FVector::DotProduct(Distance, Axis));

        // If there's a separating axis, there's no collision
        if (DistanceProjection > (AABBProjection + OBBProjection))
        {
            return false;
        }
    }

    // If no separating axis exists, the shapes are overlapping
    return true;
}

bool FTetherShape_AxisAlignedBoundingBox::OverlapWithSphere(const FTetherShape_BoundingSphere& Sphere) const
{
	FVector ClosestPoint = ClampVector(Sphere.Center, Min, Max);
	float DistanceSquared = FVector::DistSquared(ClosestPoint, Sphere.Center);
	return DistanceSquared <= FMath::Square(Sphere.Radius);
}

bool FTetherShape_AxisAlignedBoundingBox::OverlapWithCapsule(const FTetherShape_Capsule& Other) const
{
	// Step 1: Get the top and bottom points of the capsule
	FVector CapsuleTop, CapsuleBottom;
	Other.GetCapsulePoints(CapsuleTop, CapsuleBottom);

	// Step 2: Find the closest point on the capsule's line segment to the AABB
	FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(GetCenter(), CapsuleBottom, CapsuleTop);

	// Step 3: Clamp the closest point on the capsule to the AABB's extents
	FVector ClosestPointOnAABB = ClampVector(ClosestPointOnCapsule, Min, Max);

	// Step 4: Calculate the distance between the closest points on the AABB and the capsule
	float DistanceSquared = FVector::DistSquared(ClosestPointOnCapsule, ClosestPointOnAABB);

	// Step 5: Check if the distance is within the capsule's radius
	return DistanceSquared <= FMath::Square(Other.Radius);
}


bool FTetherShape_AxisAlignedBoundingBox::OverlapWithCone(const FTetherShape_Cone& Cone) const
{
	// Step 1: Calculate the tip of the cone
	// FVector ConeTip = Cone.BaseCenter + Cone.Rotation.RotateVector(FVector(0.0f, 0.0f, Cone.Height));

	// Step 2: Get all 8 vertices of the AABB
	FVector Vertices[8];
	Vertices[0] = Min;
	Vertices[1] = FVector(Min.X, Min.Y, Max.Z);
	Vertices[2] = FVector(Min.X, Max.Y, Min.Z);
	Vertices[3] = FVector(Min.X, Max.Y, Max.Z);
	Vertices[4] = FVector(Max.X, Min.Y, Min.Z);
	Vertices[5] = FVector(Max.X, Min.Y, Max.Z);
	Vertices[6] = FVector(Max.X, Max.Y, Min.Z);
	Vertices[7] = Max;

	// Step 3: Check each vertex of the AABB against the cone
	FVector ConeAxis = Cone.Rotation.RotateVector(FVector(0.0f, 0.0f, 1.0f));

	for (const FVector& Vertex : Vertices)
	{
		FVector VectorToVertex = Vertex - Cone.BaseCenter;
		float HeightAlongCone = FVector::DotProduct(VectorToVertex, ConeAxis);

		if (HeightAlongCone >= 0.0f && HeightAlongCone <= Cone.Height)
		{
			float RadiusAtHeight = (HeightAlongCone / Cone.Height) * Cone.BaseRadius;
			FVector ClosestPointOnCone = Cone.BaseCenter + ConeAxis * HeightAlongCone;

			if (FVector::DistSquared(ClosestPointOnCone, Vertex) <= FMath::Square(RadiusAtHeight))
			{
				return true;  // Overlap detected
			}
		}
	}

	return false;  // No overlap
}

void FTetherShape_AxisAlignedBoundingBox::DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color,
	bool bPersistentLines, float LifeTime, float Thickness) const
{
	FVector Vertices[8];
	Vertices[0] = Min;
	Vertices[1] = FVector(Min.X, Min.Y, Max.Z);
	Vertices[2] = FVector(Min.X, Max.Y, Min.Z);
	Vertices[3] = FVector(Min.X, Max.Y, Max.Z);
	Vertices[4] = FVector(Max.X, Min.Y, Min.Z);
	Vertices[5] = FVector(Max.X, Min.Y, Max.Z);
	Vertices[6] = FVector(Max.X, Max.Y, Min.Z);
	Vertices[7] = Max;

	// Draw edges
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[0], Vertices[1], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[0], Vertices[2], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[0], Vertices[4], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[1], Vertices[3], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[1], Vertices[5], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[2], Vertices[3], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[2], Vertices[6], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[3], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[4], Vertices[5], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[4], Vertices[6], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[5], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
	AnimInstanceProxy->AnimDrawDebugLine(Vertices[6], Vertices[7], Color, bPersistentLines, LifeTime, Thickness);
}

bool FTetherShape_BoundingSphere::CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const
{
    // Check for collision with another Bounding Sphere
    if (const FTetherShape_BoundingSphere* OtherSphere = FTetherShapeTypeCaster::CastShape<FTetherShape_BoundingSphere>(&Other))
    {
        float DistanceSquared = FVector::DistSquared(Center, OtherSphere->Center);
        float CombinedRadius = Radius + OtherSphere->Radius;

        if (DistanceSquared <= FMath::Square(CombinedRadius))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (Center + OtherSphere->Center) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = CombinedRadius - FMath::Sqrt(DistanceSquared);
            return true;
        }
    }
    // Check for collision with an Axis-Aligned Bounding Box (AABB)
    else if (const FTetherShape_AxisAlignedBoundingBox* OtherAABB = FTetherShapeTypeCaster::CastShape<FTetherShape_AxisAlignedBoundingBox>(&Other))
    {
        FVector ClosestPoint = ClampVector(Center, OtherAABB->Min, OtherAABB->Max);
        float DistanceSquared = FVector::DistSquared(ClosestPoint, Center);

        if (DistanceSquared <= FMath::Square(Radius))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = ClosestPoint;
            Output.PenetrationDepth = Radius - FMath::Sqrt(DistanceSquared);
            return true;
        }
    }
    // Check for collision with an Oriented Bounding Box (OBB)
    else if (const FTetherShape_OrientedBoundingBox* OtherOBB = FTetherShapeTypeCaster::CastShape<FTetherShape_OrientedBoundingBox>(&Other))
    {
        FVector DistanceToSphere = Center - OtherOBB->Center;

        FQuat OBBRotation = OtherOBB->Rotation.Quaternion();
        FVector OBBX = OBBRotation.GetAxisX();
        FVector OBBY = OBBRotation.GetAxisY();
        FVector OBBZ = OBBRotation.GetAxisZ();

        FVector ClosestPointOnOBB = OtherOBB->Center;

        float ProjX = FMath::Clamp(FVector::DotProduct(DistanceToSphere, OBBX), -OtherOBB->Extent.X, OtherOBB->Extent.X);
        float ProjY = FMath::Clamp(FVector::DotProduct(DistanceToSphere, OBBY), -OtherOBB->Extent.Y, OtherOBB->Extent.Y);
        float ProjZ = FMath::Clamp(FVector::DotProduct(DistanceToSphere, OBBZ), -OtherOBB->Extent.Z, OtherOBB->Extent.Z);

        ClosestPointOnOBB += OBBX * ProjX + OBBY * ProjY + OBBZ * ProjZ;

        float DistanceSquared = FVector::DistSquared(ClosestPointOnOBB, Center);

        if (DistanceSquared <= FMath::Square(Radius))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = ClosestPointOnOBB;
            Output.PenetrationDepth = Radius - FMath::Sqrt(DistanceSquared);
            return true;
        }
    }
    // Check for collision with a Capsule
    else if (const FTetherShape_Capsule* OtherCapsule = FTetherShapeTypeCaster::CastShape<FTetherShape_Capsule>(&Other))
    {
        FVector CapsuleTop, CapsuleBottom;
        OtherCapsule->GetCapsulePoints(CapsuleTop, CapsuleBottom);

        FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(Center, CapsuleBottom, CapsuleTop);
        float DistanceSquared = FVector::DistSquared(ClosestPointOnCapsule, Center);
        float CombinedRadius = Radius + OtherCapsule->Radius;

        if (DistanceSquared <= FMath::Square(CombinedRadius))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = ClosestPointOnCapsule;
            Output.PenetrationDepth = CombinedRadius - FMath::Sqrt(DistanceSquared);
            return true;
        }
    }
    // Check for collision with a Cone
    else if (const FTetherShape_Cone* OtherCone = FTetherShapeTypeCaster::CastShape<FTetherShape_Cone>(&Other))
    {
        if (OtherCone->OverlapWithSphere(*this))
        {
            Output.bHasCollision = true;
            // Use the midpoint between the sphere center and cone base center as a simple contact point
            Output.ContactPoint = (Center + OtherCone->BaseCenter) * 0.5f;
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }

    // No collision detected
    return false;
}

bool FTetherShape_BoundingSphere::OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& AABB) const
{
	FVector ClosestPoint = ClampVector(Center, AABB.Min, AABB.Max);
	float DistanceSquared = FVector::DistSquared(ClosestPoint, Center);
	return DistanceSquared <= FMath::Square(Radius);
}

bool FTetherShape_BoundingSphere::OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const
{
	// Step 1: Get the OBB's axes
	FQuat OBBRotation = Other.Rotation.Quaternion();
	FVector OBBX = OBBRotation.GetAxisX();
	FVector OBBY = OBBRotation.GetAxisY();
	FVector OBBZ = OBBRotation.GetAxisZ();

	// Step 2: Calculate the vector from the OBB center to the sphere center
	FVector DistanceToSphere = Center - Other.Center;

	// Step 3: Project the vector onto the OBB's axes to find the closest point on the OBB to the sphere center
	FVector ClosestPointOnOBB = Other.Center;

	float ProjX = FMath::Clamp(FVector::DotProduct(DistanceToSphere, OBBX), -Other.Extent.X, Other.Extent.X);
	float ProjY = FMath::Clamp(FVector::DotProduct(DistanceToSphere, OBBY), -Other.Extent.Y, Other.Extent.Y);
	float ProjZ = FMath::Clamp(FVector::DotProduct(DistanceToSphere, OBBZ), -Other.Extent.Z, Other.Extent.Z);

	ClosestPointOnOBB += OBBX * ProjX + OBBY * ProjY + OBBZ * ProjZ;

	// Step 4: Calculate the distance from the closest point on the OBB to the sphere's center
	float DistanceSquared = FVector::DistSquared(ClosestPointOnOBB, Center);

	// Step 5: Check if the distance is within the sphere's radius
	return DistanceSquared <= FMath::Square(Radius);
}

bool FTetherShape_BoundingSphere::OverlapWithCapsule(const FTetherShape_Capsule& Other) const
{
	// Step 1: Get the top and bottom points of the capsule
	FVector CapsuleTop, CapsuleBottom;
	Other.GetCapsulePoints(CapsuleTop, CapsuleBottom);

	// Step 2: Find the closest point on the capsule's line segment to the sphere's center
	FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(Center, CapsuleBottom, CapsuleTop);

	// Step 3: Calculate the distance from the closest point on the capsule to the sphere's center
	float DistanceSquared = FVector::DistSquared(ClosestPointOnCapsule, Center);

	// Step 4: Check if the distance is within the sum of the capsule's radius and the sphere's radius
	return DistanceSquared <= FMath::Square(Other.Radius + Radius);
}

bool FTetherShape_BoundingSphere::OverlapWithCone(const FTetherShape_Cone& Cone) const
{
	// Step 1: Calculate the tip of the cone
	// FVector ConeTip = Cone.BaseCenter + Cone.Rotation.RotateVector(FVector(0.0f, 0.0f, Cone.Height));

	// Step 2: Calculate the vector from the cone's base center to the sphere's center
	FVector ToSphere = Center - Cone.BaseCenter;

	// Step 3: Project this vector onto the cone's axis to find the height along the cone
	FVector ConeAxis = Cone.Rotation.RotateVector(FVector(0.0f, 0.0f, 1.0f));
	float HeightAlongCone = FVector::DotProduct(ToSphere, ConeAxis);

	// Step 4: Check if the height is within the cone's height range
	if (HeightAlongCone < 0.0f || HeightAlongCone > Cone.Height)
	{
		return false;  // No overlap
	}

	// Step 5: Calculate the radius of the cone at this height
	float RadiusAtHeight = (HeightAlongCone / Cone.Height) * Cone.BaseRadius;

	// Step 6: Find the closest point on the cone's axis to the sphere's center
	FVector ClosestPointOnCone = Cone.BaseCenter + ConeAxis * HeightAlongCone;

	// Step 7: Check if the sphere overlaps with the cone at this height
	return FVector::DistSquared(ClosestPointOnCone, Center) <= FMath::Square(RadiusAtHeight + Radius);
}

void FTetherShape_BoundingSphere::DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color,
	bool bPersistentLines, float LifeTime, float Thickness) const
{
	AnimInstanceProxy->AnimDrawDebugSphere(Center, Radius, 12, Color, bPersistentLines, LifeTime, Thickness);
}

bool FTetherShape_OrientedBoundingBox::CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const
{
    // Check for collision with another Oriented Bounding Box (OBB)
    if (const FTetherShape_OrientedBoundingBox* OtherOBB = FTetherShapeTypeCaster::CastShape<FTetherShape_OrientedBoundingBox>(&Other))
    {
        if (OverlapWithOBB(*OtherOBB))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (Center + OtherOBB->Center) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with an Axis-Aligned Bounding Box (AABB)
    else if (const FTetherShape_AxisAlignedBoundingBox* OtherAABB = FTetherShapeTypeCaster::CastShape<FTetherShape_AxisAlignedBoundingBox>(&Other))
    {
        if (OverlapWithAABB(*OtherAABB))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (Center + OtherAABB->GetCenter()) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with a Bounding Sphere
    else if (const FTetherShape_BoundingSphere* OtherSphere = FTetherShapeTypeCaster::CastShape<FTetherShape_BoundingSphere>(&Other))
    {
        FVector DistanceToSphere = OtherSphere->Center - Center;

        FQuat OBBRotation = Rotation.Quaternion();
        FVector OBBX = OBBRotation.GetAxisX();
        FVector OBBY = OBBRotation.GetAxisY();
        FVector OBBZ = OBBRotation.GetAxisZ();

        FVector ClosestPointOnOBB = Center;

        float ProjX = FMath::Clamp(FVector::DotProduct(DistanceToSphere, OBBX), -Extent.X, Extent.X);
        float ProjY = FMath::Clamp(FVector::DotProduct(DistanceToSphere, OBBY), -Extent.Y, Extent.Y);
        float ProjZ = FMath::Clamp(FVector::DotProduct(DistanceToSphere, OBBZ), -Extent.Z, Extent.Z);

        ClosestPointOnOBB += OBBX * ProjX + OBBY * ProjY + OBBZ * ProjZ;

        float DistanceSquared = FVector::DistSquared(ClosestPointOnOBB, OtherSphere->Center);

        if (DistanceSquared <= FMath::Square(OtherSphere->Radius))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = ClosestPointOnOBB;
            Output.PenetrationDepth = OtherSphere->Radius - FMath::Sqrt(DistanceSquared);
            return true;
        }
    }
    // Check for collision with a Capsule
    else if (const FTetherShape_Capsule* OtherCapsule = FTetherShapeTypeCaster::CastShape<FTetherShape_Capsule>(&Other))
    {
        if (OverlapWithCapsule(*OtherCapsule))
        {
            Output.bHasCollision = true;
            FVector CapsuleTop, CapsuleBottom;
            OtherCapsule->GetCapsulePoints(CapsuleTop, CapsuleBottom);
            FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(Center, CapsuleBottom, CapsuleTop);
            Output.ContactPoint = ClosestPointOnCapsule;
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with a Cone
    else if (const FTetherShape_Cone* OtherCone = FTetherShapeTypeCaster::CastShape<FTetherShape_Cone>(&Other))
    {
        if (OtherCone->OverlapWithOBB(*this))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (Center + OtherCone->BaseCenter) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }

    // No collision detected
    return false;
}

bool FTetherShape_OrientedBoundingBox::OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& Other) const
{
    // Step 1: Compute the half-extents of the AABB
    FVector AABBCenter = Other.GetCenter();
    FVector AABBExtents = (Other.Max - Other.Min) * 0.5f;

    // Step 2: Get the OBB's axes and half-extents
    FVector OBBHalfExtents = Extent;

    FQuat OBBRotation = Rotation.Quaternion();

    FVector OBBX = OBBRotation.GetAxisX();
    FVector OBBY = OBBRotation.GetAxisY();
    FVector OBBZ = OBBRotation.GetAxisZ();

    FVector OBBCenter = Center;

    // Step 3: Compute the distance vector between the centers
    FVector Distance = AABBCenter - OBBCenter;

    // Step 4: Check each axis (AABB's axes and OBB's axes)
    TArray<FVector> Axes = {
        FVector::ForwardVector, // AABB X-axis
        FVector::RightVector,   // AABB Y-axis
        FVector::UpVector,      // AABB Z-axis
        OBBX,
        OBBY,
        OBBZ
    };

    for (const FVector& Axis : Axes)
    {
        // Cache the absolute values of the axis vector
        FVector AbsAxis = Axis.GetAbs();

        // Project the half-extents onto the axis
        float AABBProjection = FVector::DotProduct(AABBExtents * AbsAxis, FVector::ForwardVector) +
                               FVector::DotProduct(AABBExtents * AbsAxis, FVector::RightVector) +
                               FVector::DotProduct(AABBExtents * AbsAxis, FVector::UpVector);

        float OBBProjection = FVector::DotProduct(OBBX * OBBHalfExtents.X, Axis) +
                              FVector::DotProduct(OBBY * OBBHalfExtents.Y, Axis) +
                              FVector::DotProduct(OBBZ * OBBHalfExtents.Z, Axis);

        float DistanceProjection = FMath::Abs(FVector::DotProduct(Distance, Axis));

        // If there's a separating axis, there's no collision
        if (DistanceProjection > (AABBProjection + OBBProjection))
        {
            return false;
        }
    }

    // If no separating axis exists, the shapes are overlapping
    return true;
}

bool FTetherShape_OrientedBoundingBox::OverlapWithSphere(const FTetherShape_BoundingSphere& Sphere) const
{
	// Simplified check using an AABB approximation of the OBB
	FTetherShape_AxisAlignedBoundingBox AABB = GenerateAABB(*this);
	return AABB.OverlapWithSphere(Sphere);
}
bool FTetherShape_OrientedBoundingBox::OverlapWithCapsule(const FTetherShape_Capsule& Other) const
{
	// Step 1: Get the top and bottom points of the capsule
	FVector CapsuleTop, CapsuleBottom;
	Other.GetCapsulePoints(CapsuleTop, CapsuleBottom);

	// Step 2: Find the closest point on the capsule's line segment to the OBB
	FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(Center, CapsuleBottom, CapsuleTop);

	// Step 3: Calculate the OBB's axes
	FQuat OBBRotation = Rotation.Quaternion();
	FVector OBBX = OBBRotation.GetAxisX();
	FVector OBBY = OBBRotation.GetAxisY();
	FVector OBBZ = OBBRotation.GetAxisZ();

	// Step 4: Calculate the distance from the closest point on the capsule to the OBB
	FVector DistanceToCapsule = ClosestPointOnCapsule - Center;

	// Step 5: Project the distance vector onto the OBB's axes and check against the OBB's extents
	// FVector AbsDistanceX = OBBX.GetAbs();
	// FVector AbsDistanceY = OBBY.GetAbs();
	// FVector AbsDistanceZ = OBBZ.GetAbs();

	float ProjX = FMath::Abs(FVector::DotProduct(DistanceToCapsule, OBBX)) - Extent.X;
	float ProjY = FMath::Abs(FVector::DotProduct(DistanceToCapsule, OBBY)) - Extent.Y;
	float ProjZ = FMath::Abs(FVector::DotProduct(DistanceToCapsule, OBBZ)) - Extent.Z;

	// If the projection is negative on all axes, the point is inside the OBB
	if (ProjX <= 0.0f && ProjY <= 0.0f && ProjZ <= 0.0f)
	{
		return true;  // Overlap detected
	}

	// Step 6: Calculate the square distance from the OBB surface to the capsule's closest point
	float SquareDistance = FMath::Max(ProjX, 0.0f) * FMath::Max(ProjX, 0.0f) +
						   FMath::Max(ProjY, 0.0f) * FMath::Max(ProjY, 0.0f) +
						   FMath::Max(ProjZ, 0.0f) * FMath::Max(ProjZ, 0.0f);

	// Step 7: Check if the square distance is within the capsule's radius squared
	return SquareDistance <= FMath::Square(Other.Radius);
}

bool FTetherShape_OrientedBoundingBox::OverlapWithCone(const FTetherShape_Cone& Cone) const
{
	// Step 1: Get all 8 vertices of the OBB
	TArray<FVector> Vertices = GetVertices();
	FVector ConeAxis = Cone.Rotation.RotateVector(FVector(0.0f, 0.0f, 1.0f));

	// Step 2: Check each vertex of the OBB against the cone
	for (const FVector& Vertex : Vertices)
	{
		FVector VectorToVertex = Vertex - Cone.BaseCenter;
		float HeightAlongCone = FVector::DotProduct(VectorToVertex, ConeAxis);

		if (HeightAlongCone >= 0.0f && HeightAlongCone <= Cone.Height)
		{
			float RadiusAtHeight = (HeightAlongCone / Cone.Height) * Cone.BaseRadius;
			FVector ClosestPointOnCone = Cone.BaseCenter + ConeAxis * HeightAlongCone;

			if (FVector::DistSquared(ClosestPointOnCone, Vertex) <= FMath::Square(RadiusAtHeight))
			{
				return true;  // Overlap detected
			}
		}
	}

	return false;  // No overlap
}

void FTetherShape_OrientedBoundingBox::DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color,
	bool bPersistentLines, float LifeTime, float Thickness) const
{
	TArray<FVector> Vertices = GetVertices();
    
	for (int32 i = 0; i < 4; ++i)
	{
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[i], Vertices[(i + 1) % 4], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[i + 4], Vertices[(i + 1) % 4 + 4], Color, bPersistentLines, LifeTime, Thickness);
		AnimInstanceProxy->AnimDrawDebugLine(Vertices[i], Vertices[i + 4], Color, bPersistentLines, LifeTime, Thickness);
	}
}

bool FTetherShape_Capsule::CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const
{
    // Check for collision with another Capsule
    if (const FTetherShape_Capsule* OtherCapsule = FTetherShapeTypeCaster::CastShape<FTetherShape_Capsule>(&Other))
    {
        FVector ClosestPoint1, ClosestPoint2;
        FMath::SegmentDistToSegmentSafe(
        	Center + FVector(0, 0, HalfHeight), Center - FVector(0, 0, HalfHeight), 
            OtherCapsule->Center + FVector(0, 0, OtherCapsule->HalfHeight), 
            OtherCapsule->Center - FVector(0, 0, OtherCapsule->HalfHeight), 
            ClosestPoint1, ClosestPoint2);

        float DistanceSquared = FVector::DistSquared(ClosestPoint1, ClosestPoint2);
        float CombinedRadius = Radius + OtherCapsule->Radius;

        if (DistanceSquared <= FMath::Square(CombinedRadius))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (ClosestPoint1 + ClosestPoint2) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = CombinedRadius - FMath::Sqrt(DistanceSquared);
            return true;
        }
    }
    // Check for collision with a Bounding Sphere
    else if (const FTetherShape_BoundingSphere* OtherSphere = FTetherShapeTypeCaster::CastShape<FTetherShape_BoundingSphere>(&Other))
    {
        FVector CapsuleTop, CapsuleBottom;
        GetCapsulePoints(CapsuleTop, CapsuleBottom);
        FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(OtherSphere->Center, CapsuleBottom, CapsuleTop);
        float DistanceSquared = FVector::DistSquared(ClosestPointOnCapsule, OtherSphere->Center);
        float CombinedRadius = Radius + OtherSphere->Radius;

        if (DistanceSquared <= FMath::Square(CombinedRadius))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = ClosestPointOnCapsule;
            Output.PenetrationDepth = CombinedRadius - FMath::Sqrt(DistanceSquared);
            return true;
        }
    }
    // Check for collision with an Axis-Aligned Bounding Box (AABB)
    else if (const FTetherShape_AxisAlignedBoundingBox* OtherAABB = FTetherShapeTypeCaster::CastShape<FTetherShape_AxisAlignedBoundingBox>(&Other))
    {
        if (OverlapWithAABB(*OtherAABB))
        {
            Output.bHasCollision = true;
            FVector ClosestPoint = ClampVector(Center, OtherAABB->Min, OtherAABB->Max);
            Output.ContactPoint = ClosestPoint;
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with an Oriented Bounding Box (OBB)
    else if (const FTetherShape_OrientedBoundingBox* OtherOBB = FTetherShapeTypeCaster::CastShape<FTetherShape_OrientedBoundingBox>(&Other))
    {
        if (OverlapWithOBB(*OtherOBB))
        {
            Output.bHasCollision = true;
            FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(Center, OtherOBB->Center + OtherOBB->Rotation.RotateVector(FVector(0, 0, OtherOBB->Extent.Z)), 
                                                                         OtherOBB->Center - OtherOBB->Rotation.RotateVector(FVector(0, 0, OtherOBB->Extent.Z)));
            Output.ContactPoint = ClosestPointOnCapsule;
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with a Cone
    else if (const FTetherShape_Cone* OtherCone = FTetherShapeTypeCaster::CastShape<FTetherShape_Cone>(&Other))
    {
        if (OtherCone->OverlapWithCapsule(*this))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (Center + OtherCone->BaseCenter) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }

    // No collision detected
    return false;
}

bool FTetherShape_Capsule::OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& Other) const
{
	// Step 1: Get the top and bottom points of the capsule
	FVector CapsuleTop, CapsuleBottom;
	GetCapsulePoints(CapsuleTop, CapsuleBottom);

	// Step 2: Find the closest point on the capsule's line segment to the AABB
	FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(Other.GetCenter(), CapsuleBottom, CapsuleTop);

	// Step 3: Find the closest point on the AABB to the closest point on the capsule
	FVector ClosestPointOnAABB = ClampVector(ClosestPointOnCapsule, Other.Min, Other.Max);

	// Step 4: Calculate the distance between the closest points on the capsule and the AABB
	float DistanceSquared = FVector::DistSquared(ClosestPointOnCapsule, ClosestPointOnAABB);

	// Step 5: Check if the distance is within the capsule's radius
	return DistanceSquared <= FMath::Square(Radius);
}

bool FTetherShape_Capsule::OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const
{
	// Step 1: Get the top and bottom points of the capsule
	FVector CapsuleTop, CapsuleBottom;
	GetCapsulePoints(CapsuleTop, CapsuleBottom);

	// Step 2: Find the closest point on the capsule's line segment to the OBB
	FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(Other.Center, CapsuleBottom, CapsuleTop);

	// Step 3: Get the OBB's axes
	FQuat OBBRotation = Other.Rotation.Quaternion();
	FVector OBBX = OBBRotation.GetAxisX();
	FVector OBBY = OBBRotation.GetAxisY();
	FVector OBBZ = OBBRotation.GetAxisZ();

	// Step 4: Project the distance vector onto the OBB's axes to find the closest point on the OBB
	FVector DistanceToCapsule = ClosestPointOnCapsule - Other.Center;
	FVector ClosestPointOnOBB = Other.Center;

	float ProjX = FMath::Clamp(FVector::DotProduct(DistanceToCapsule, OBBX), -Other.Extent.X, Other.Extent.X);
	float ProjY = FMath::Clamp(FVector::DotProduct(DistanceToCapsule, OBBY), -Other.Extent.Y, Other.Extent.Y);
	float ProjZ = FMath::Clamp(FVector::DotProduct(DistanceToCapsule, OBBZ), -Other.Extent.Z, Other.Extent.Z);

	ClosestPointOnOBB += OBBX * ProjX + OBBY * ProjY + OBBZ * ProjZ;

	// Step 5: Calculate the distance between the closest points on the capsule and the OBB
	float DistanceSquared = FVector::DistSquared(ClosestPointOnCapsule, ClosestPointOnOBB);

	// Step 6: Check if the distance is within the capsule's radius
	return DistanceSquared <= FMath::Square(Radius);
}

bool FTetherShape_Capsule::OverlapWithSphere(const FTetherShape_BoundingSphere& Sphere) const
{
	FTetherShape_BoundingSphere BoundingSphere = GenerateBoundingSphere();
	return BoundingSphere.OverlapWithSphere(Sphere);
}

bool FTetherShape_Capsule::OverlapWithCone(const FTetherShape_Cone& Cone) const
{
	// Step 1: Calculate the tip of the cone
	// FVector ConeTip = Cone.BaseCenter + Cone.Rotation.RotateVector(FVector(0.0f, 0.0f, Cone.Height));

	// Step 2: Get the top and bottom points of the capsule
	FVector CapsuleTop, CapsuleBottom;
	GetCapsulePoints(CapsuleTop, CapsuleBottom);

	// Step 3: Find the closest point on the capsule line segment to the cone's base center
	FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(Cone.BaseCenter, CapsuleBottom, CapsuleTop);

	// Step 4: Calculate the vector from the cone's base center to this closest point
	FVector VectorToCapsule = ClosestPointOnCapsule - Cone.BaseCenter;

	// Step 5: Project this vector onto the cone's axis to find the height along the cone
	FVector ConeAxis = Cone.Rotation.RotateVector(FVector(0.0f, 0.0f, 1.0f));
	float HeightAlongCone = FVector::DotProduct(VectorToCapsule, ConeAxis);

	// Step 6: Check if this height is within the cone's height range
	if (HeightAlongCone < 0.0f || HeightAlongCone > Cone.Height)
	{
		return false;  // No overlap
	}

	// Step 7: Calculate the radius of the cone at this height
	float ConeRadiusAtHeight = (HeightAlongCone / Cone.Height) * Cone.BaseRadius;

	// Step 8: Calculate the distance from the capsule to the cone's axis at this height
	float DistanceToCapsule = FVector::Dist(ClosestPointOnCapsule, Cone.BaseCenter + ConeAxis * HeightAlongCone);

	// Step 9: Check if this distance is within the sum of the cone's radius at this height and the capsule's radius
	return DistanceToCapsule <= (ConeRadiusAtHeight + Radius);
}

void FTetherShape_Capsule::DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines,
	float LifeTime, float Thickness) const
{
	AnimInstanceProxy->AnimDrawDebugCapsule(Center, HalfHeight, Radius, Rotation, Color, bPersistentLines, LifeTime, Thickness);
}

bool FTetherShape_Cone::CheckNarrowCollision(const FTetherShape& Other, FTetherNarrowPhaseCollisionOutput& Output) const
{
    // Check for collision with another Cone (using a simplified method)
    if (const FTetherShape_Cone* OtherCone = FTetherShapeTypeCaster::CastShape<FTetherShape_Cone>(&Other))
    {
        if (OverlapWithCone(*OtherCone))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (BaseCenter + OtherCone->BaseCenter) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with a Bounding Sphere
    else if (const FTetherShape_BoundingSphere* OtherSphere = FTetherShapeTypeCaster::CastShape<FTetherShape_BoundingSphere>(&Other))
    {
        if (OverlapWithSphere(*OtherSphere))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (BaseCenter + OtherSphere->Center) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with an Axis-Aligned Bounding Box (AABB)
    else if (const FTetherShape_AxisAlignedBoundingBox* OtherAABB = FTetherShapeTypeCaster::CastShape<FTetherShape_AxisAlignedBoundingBox>(&Other))
    {
        if (OverlapWithAABB(*OtherAABB))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (BaseCenter + OtherAABB->GetCenter()) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with an Oriented Bounding Box (OBB)
    else if (const FTetherShape_OrientedBoundingBox* OtherOBB = FTetherShapeTypeCaster::CastShape<FTetherShape_OrientedBoundingBox>(&Other))
    {
        if (OverlapWithOBB(*OtherOBB))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (BaseCenter + OtherOBB->Center) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }
    // Check for collision with a Capsule
    else if (const FTetherShape_Capsule* OtherCapsule = FTetherShapeTypeCaster::CastShape<FTetherShape_Capsule>(&Other))
    {
        if (OverlapWithCapsule(*OtherCapsule))
        {
            Output.bHasCollision = true;
            Output.ContactPoint = (BaseCenter + OtherCapsule->Center) * 0.5f; // Midpoint as contact point
            Output.PenetrationDepth = 0.0f; // Placeholder value (can be refined)
            return true;
        }
    }

    // No collision detected
    return false;
}

bool FTetherShape_Cone::OverlapWithAABB(const FTetherShape_AxisAlignedBoundingBox& Other) const
{
	// A simplified approach: Check if the AABB's center is within the cone's volume
	FVector ClosestPoint = ClampVector(Other.GetCenter(), BaseCenter - FVector(BaseRadius, BaseRadius, 0), BaseCenter + FVector(BaseRadius, BaseRadius, Height));

	// Check the distance from the base along the cone's direction
	FVector ToPoint = ClosestPoint - BaseCenter;
	float DistanceAlongCone = FVector::DotProduct(ToPoint, Rotation.RotateVector(FVector(0.0f, 0.0f, 1.0f)));

	// Calculate the radius at this height along the cone
	float RadiusAtHeight = (DistanceAlongCone / Height) * BaseRadius;

	return ToPoint.SizeSquared() <= FMath::Square(RadiusAtHeight);
}

bool FTetherShape_Cone::OverlapWithOBB(const FTetherShape_OrientedBoundingBox& Other) const
{
	// Simplified approach: Check each vertex of the OBB to see if it's inside the cone
	TArray<FVector> Vertices = Other.GetVertices();
	FVector ConeDirection = Rotation.RotateVector(FVector(0.0f, 0.0f, 1.0f));

	for (const FVector& Vertex : Vertices)
	{
		FVector ToVertex = Vertex - BaseCenter;
		float HeightAlongCone = FVector::DotProduct(ToVertex, ConeDirection);

		if (HeightAlongCone >= 0.0f && HeightAlongCone <= Height)
		{
			float RadiusAtHeight = (HeightAlongCone / Height) * BaseRadius;
			FVector ClosestPointOnCone = BaseCenter + ConeDirection * HeightAlongCone;

			if (FVector::DistSquared(ClosestPointOnCone, Vertex) <= FMath::Square(RadiusAtHeight))
			{
				return true; // At least one vertex is inside the cone
			}
		}
	}

	return false;
}

bool FTetherShape_Cone::OverlapWithSphere(const FTetherShape_BoundingSphere& Other) const
{
	// Get the vector from the cone's base to the sphere's center
	FVector ToSphere = Other.Center - BaseCenter;
    
	// Project this vector onto the cone's direction to get the height along the cone
	float HeightAlongCone = FVector::DotProduct(ToSphere, Rotation.RotateVector(FVector(0.0f, 0.0f, 1.0f)));

	// If the height is negative or exceeds the cone's height, the sphere is outside the cone
	if (HeightAlongCone < 0.0f || HeightAlongCone > Height)
	{
		return false;
	}

	// Calculate the radius of the cone at this height
	float RadiusAtHeight = (HeightAlongCone / Height) * BaseRadius;

	// Find the closest point on the cone's surface to the sphere's center
	FVector ClosestPointOnCone = BaseCenter + Rotation.RotateVector(FVector(0.0f, 0.0f, HeightAlongCone));
	FVector ClosestToSphere = Other.Center - ClosestPointOnCone;

	// Check if the sphere's center is within the radius at this height
	return ClosestToSphere.SizeSquared() <= FMath::Square(RadiusAtHeight + Other.Radius);
}

bool FTetherShape_Cone::OverlapWithCapsule(const FTetherShape_Capsule& Other) const
{
	// Step 1: Calculate the tip of the cone
	// FVector ConeTip = BaseCenter + Rotation.RotateVector(FVector(0.0f, 0.0f, Height));

	// Step 2: Calculate the axis vector of the cone and the capsule
	FVector ConeAxis = Rotation.RotateVector(FVector::UpVector);
	// FVector CapsuleAxis = Other.Rotation.RotateVector(FVector::UpVector);

	// Step 3: Calculate the top and bottom points of the capsule
	FVector CapsuleTop, CapsuleBottom;
	Other.GetCapsulePoints(CapsuleTop, CapsuleBottom);

	// Step 4: Find the closest point on the capsule line segment to the cone's base center
	FVector ClosestPointOnCapsule = FMath::ClosestPointOnSegment(BaseCenter, CapsuleBottom, CapsuleTop);

	// Step 5: Calculate the vector from the cone's base center to this closest point
	FVector VectorToCapsule = ClosestPointOnCapsule - BaseCenter;

	// Step 6: Project this vector onto the cone's axis to find the height along the cone
	float HeightAlongCone = FVector::DotProduct(VectorToCapsule, ConeAxis);

	// Step 7: Check if this height is within the cone's height range
	if (HeightAlongCone < 0.0f || HeightAlongCone > Height)
	{
		return false;  // No overlap if the closest point is outside the cone's height
	}

	// Step 8: Calculate the radius of the cone at this height
	float ConeRadiusAtHeight = (HeightAlongCone / Height) * BaseRadius;

	// Step 9: Calculate the distance from the capsule line segment to the cone's axis at this height
	float DistanceToCapsule = FVector::Dist(ClosestPointOnCapsule, BaseCenter + ConeAxis * HeightAlongCone);

	// Step 10: Check if this distance is within the sum of the cone's radius at this height and the capsule's radius
	if (DistanceToCapsule <= (ConeRadiusAtHeight + Other.Radius))
	{
		return true;  // Overlap detected
	}

	return false;  // No overlap
}

void FTetherShape_Cone::DrawDebug(FAnimInstanceProxy* AnimInstanceProxy, const FColor& Color, bool bPersistentLines,
	float LifeTime, float Thickness) const
{
	// Calculate the tip of the cone (retained for clarity or future use)
	// const FVector Tip = BaseCenter + Rotation.RotateVector(FVector(0.0f, 0.0f, Height));

	// Direction vector along the height of the cone
	const FVector Direction = Rotation.RotateVector(FVector::UpVector);

	// Calculate the angles for the cone's base using the FMath library
	const float AngleWidth = FMath::Atan(BaseRadius / Height);
	const float AngleHeight = AngleWidth;  // Since it's a circular base, the angles are the same

	// Draw the debug cone
	AnimInstanceProxy->AnimDrawDebugCone(BaseCenter, BaseRadius, Direction, AngleWidth, AngleHeight, 16, Color, bPersistentLines, LifeTime, SDPG_World, Thickness);
}
