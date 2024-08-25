// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "TetherEditorShapeActor.h"

#include "TetherDeveloperSettings.h"
#include "TetherEditorShapeAnimInstance.h"
#include "Tether/Public/TetherIO.h"
#include "Shapes/TetherShape.h"

ATetherEditorShapeActor::ATetherEditorShapeActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsEditorOnlyActor = true;

	// These are ticked by TetherEditorSubsystem
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DebugMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DebugMesh"));
	SetRootComponent(DebugMesh);
	DebugMesh->SetVisibility(false);
	DebugMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	DebugMesh->SetAnimInstanceClass(UTetherEditorShapeAnimInstance::StaticClass());
}

FTetherShape& ATetherEditorShapeActor::GetTetherShape() const
{
	if (ShapeType == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox) { return AABB; }
	// if (ShapeType == FTetherGameplayTags::Tether_Shape_OrientedBoundingBox) { return OBB; }
	// if (ShapeType == FTetherGameplayTags::Tether_Shape_BoundingSphere) { return BoundingSphere; }
	// if (ShapeType == FTetherGameplayTags::Tether_Shape_Capsule) { return Capsule; }
	// if (ShapeType == FTetherGameplayTags::Tether_Shape_Cone) { return Cone; }
	
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
	const UTetherShapeCollisionControl* Control = Settings->ShapeCollisionControl.GetDefaultObject();

	if (!IsValid(OtherShapeActor))
	{
		return;
	}
	
	const FTetherShape OtherShape = OtherShapeActor->GetTetherShape();
	
	if (GetTetherShape().IsValid() && OtherShape.IsValid())
	{
		// Apply the actor's transform to the shape
		FTransform ActorTransform = GetActorTransform();
		FTetherShape TransformedShape = GetTetherShape();  // Copying means we don't have to convert it back after we're done
		TransformedShape.GetTetherShape()->TransformToWorldSpace(TransformedShape, ActorTransform);
	
		FTransform OtherActorTransform = OtherShapeActor->GetActorTransform(); // Assuming OtherShape is linked to an actor
		FTetherShape TransformedOtherShape = OtherShape;  // Copying means we don't have to convert it back after we're done
		OtherShape.GetTetherShape()->TransformToWorldSpace(OtherShape, OtherActorTransform);
	
		// Get the shape from the other actor
		FTetherNarrowPhaseCollisionOutput CollisionOutput;
		bool bHasCollision = TransformedShape.CheckNarrowCollision(OtherShape, CollisionOutput);
	
		if (bHasCollision)
		{
			// Draw debug point at collision contact point
			DrawDebugPoint(GetWorld(), CollisionOutput.ContactPoint, 10.0f, FColor::Red, false, -1.f);
	
			// Optionally draw penetration depth line
			FVector PenetrationEnd = CollisionOutput.ContactPoint + FVector(0, 0, CollisionOutput.PenetrationDepth);
			DrawDebugLine(GetWorld(), CollisionOutput.ContactPoint, PenetrationEnd, FColor::Red, false, -1.f, 0, 2.0f);
	
			// Draw the shapes in their colliding color (e.g., red)
			TransformedShape.GetTetherShape()->DrawDebug(GetAnimInstanceProxy(), FColor::Red);
			TransformedOtherShape.GetTetherShape()->DrawDebug(GetAnimInstanceProxy(), FColor::Red);
		}
		else
		{
			// Draw the shapes in their non-colliding color (e.g., green)
			TransformedShape.GetTetherShape()->DrawDebug(GetAnimInstanceProxy(), FColor::Green);
			TransformedOtherShape.GetTetherShape()->DrawDebug(GetAnimInstanceProxy(), FColor::Green);
		}
	}
}

FAnimInstanceProxy* ATetherEditorShapeActor::GetAnimInstanceProxy() const
{
	// We need a non-const proxy, anim instance only gives us const
	UTetherEditorShapeAnimInstance* AnimInstance = CastChecked<UTetherEditorShapeAnimInstance>(DebugMesh->GetAnimInstance());
	const FAnimInstanceProxy* Proxy = &AnimInstance->GetAnimInstanceProxy();
	return const_cast<FAnimInstanceProxy*>(Proxy);
}

bool ATetherEditorShapeActor::CanEditChange(const FProperty* InProperty) const
{
	if (InProperty->GetFName().IsEqual(GET_MEMBER_NAME_CHECKED(ThisClass, AABB)))
	{
		return ShapeType == FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox;
	}
	return Super::CanEditChange(InProperty);
}
