// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherEditorShapeActor.h"

#include "TetherDeveloperSettings.h"
#include "TetherIO.h"
#include "Physics/Collision/TetherCollisionDetectionHandler.h"
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

const FTetherShape& ATetherEditorShapeActor::GetTetherShape() const
{
	if (ShapeType == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox) { return AABB; }
	if (ShapeType == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox) { return OBB; }
	if (ShapeType == FTetherGameplayTags::Tether_Shape_BoundingSphere) { return BoundingSphere; }
	if (ShapeType == FTetherGameplayTags::Tether_Shape_Capsule) { return Capsule; }
	
	return AABB;
}

void ATetherEditorShapeActor::TestCollisions(const TArray<ATetherEditorShapeActor*>& OtherShapeActors) const
{
	for (const ATetherEditorShapeActor* OtherActor : OtherShapeActors)
	{
		if (OtherActor != this)
		{
			TestCollision(OtherActor);
		}
	}
}

void ATetherEditorShapeActor::TestCollision(const ATetherEditorShapeActor* OtherShapeActor) const
{
	const UTetherDeveloperSettings* Settings = GetDefault<UTetherDeveloperSettings>();
	const UTetherCollisionDetectionHandler* Control = Settings->CollisionDetectionHandler.GetDefaultObject();
	
	if (!IsValid(OtherShapeActor))
	{
		return;
	}
	
	if (GetTetherShape().IsValid() && OtherShapeActor->GetTetherShape().IsValid())
	{
		// Apply the actor's transform to the shape
		FTransform ActorTransform = GetActorTransform();
		TSharedPtr<FTetherShape> TransformedShape = GetTetherShape().Clone();  // Copying means we don't have to convert it back after we're done
		TransformedShape->ToWorldSpace(ActorTransform);
	
		FTransform OtherActorTransform = OtherShapeActor->GetActorTransform(); // Assuming OtherShape is linked to an actor
		TSharedPtr<FTetherShape> TransformedOtherShape = OtherShapeActor->GetTetherShape().Clone();  // Copying means we don't have to convert it back after we're done
		TransformedOtherShape->ToWorldSpace(OtherActorTransform);
	
		// Get the shape from the other actor
		FTetherNarrowPhaseCollisionOutput CollisionOutput;
		const bool bHasCollision = Control->CheckNarrowCollision(*TransformedShape, *TransformedOtherShape, CollisionOutput);
	
		static constexpr float DebugThickness = 0.0f;
		
		if (bHasCollision)
		{
			// Draw debug point at collision contact point
			DrawDebugPoint(GetWorld(), CollisionOutput.ContactPoint, 10.0f, FColor::Red, false, -1.f);
	
			// Optionally draw penetration depth line
			FVector PenetrationEnd = CollisionOutput.ContactPoint + FVector(0, 0, CollisionOutput.PenetrationDepth);
			DrawDebugLine(GetWorld(), CollisionOutput.ContactPoint, PenetrationEnd, FColor::Red, false, -1.f, 0, 2.0f);
	
			// Draw the shapes in their colliding color (e.g., red)
			TransformedShape->DrawDebug(GetWorld(), FColor::Red, false, -1, DebugThickness);
			TransformedOtherShape->DrawDebug(GetWorld(), FColor::Red, false, -1, DebugThickness);
		}
		else
		{
			// Draw the shapes in their non-colliding color (e.g., green)
			TransformedShape->DrawDebug(GetWorld(), FColor::Green, false, -1, DebugThickness);
			TransformedOtherShape->DrawDebug(GetWorld(), FColor::Green, false, -1, DebugThickness);
		}
	}
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
