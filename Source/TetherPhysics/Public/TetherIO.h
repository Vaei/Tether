// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shapes/TetherShape.h"

#include "TetherIO.generated.h"


UENUM(BlueprintType)
enum class ETetherDampingModel : uint8
{
	SimpleLinear			UMETA(ToolTip="Subtracts a portion of angular velocity (a) each frame, based on damping coefficient (k) and time step (d): a *= (1 - k * d)"),
	ExponentialDecay		UMETA(ToolTip="Angular velocity (a) decreases over time with rate of decrease proportional to current velocity. More realistic in many physical systems, especially for simulating air resistance or other forms of damping that don't depend linearly on velocity: a *= exp(-k * d)"),
};

USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FLinearInput
{
	GENERATED_BODY()

	FLinearInput()
		: Force(FVector::ZeroVector)						// Default to no initial force
		, Mass(1.f)											// Default to a mass of 1.0 for reasonable physics interaction
		, LinearDamping(0.05f)								// A small default damping to simulate some resistance to linear motion
		, MaxLinearVelocity(1000.f)							// A high cap on linear velocity to prevent instability
		, FrictionForce(0.f)								// Default to no additional frictional force
		, LinearDragCoefficient(0.01f)						// A small drag coefficient to account for air resistance
		, DampingModel(ETetherDampingModel::SimpleLinear)	// Default to simple linear damping
	{}

	/** Applied force causing linear acceleration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Force;

	/** Mass of the object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float Mass;

	/** Resistance to motion, slows down linear velocity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float LinearDamping;

	/** Cap linear velocity to avoid unreasonable speeds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float MaxLinearVelocity;

	/** Additional force due to frictional forces */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float FrictionForce;

	/** Coefficient for drag forces that oppose motion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float LinearDragCoefficient;

	/** Toggle to apply advanced damping effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	ETetherDampingModel DampingModel;
};

USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FLinearOutput
{
	GENERATED_BODY()

	FLinearOutput()
		: LinearVelocity(FVector::ZeroVector)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector LinearVelocity;
};

USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FAngularInput
{
	GENERATED_BODY()

	FAngularInput()
		: Torque(FVector::ZeroVector)						// No initial torque
		, PointOfApplication(FVector::ZeroVector)			// Origin (center of the object)
		, CenterOfMass(FVector::ZeroVector)					// Geometric center of the object
		, bUseDynamicInertia(true)							// Dynamic inertia calculation based on the object's dimensions
		, Inertia(FVector::OneVector)						// Reasonable non-zero default for static inertia
		, Mass(1.f)											// Mass of 1.0 for reasonable physics interaction
		, AngularDamping(0.05f)								// Small default damping to simulate some resistance to rotation
		, MaxAngularVelocity(1000.f)						// High cap on angular velocity to prevent instability
		, FrictionTorque(0.f)								// Default to no additional frictional torque
		, AngularDragCoefficient(0.01f)						// Small drag coefficient to account for air resistance
		, DampingModel(ETetherDampingModel::SimpleLinear)	// Simple linear damping
	{}

	/** Applied torque causing angular acceleration */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Torque;
	
	/** Point where the force is applied, offset relative to the object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector PointOfApplication;
	
	/** Center of mass of the object, offset relative to the object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector CenterOfMass;
	
	/** If enabled, factors in the extents of the object when computing Inertia */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	bool bUseDynamicInertia;

	/** Center of mass of the object, offset relative to the object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(EditCondition="!bUseDynamicInertia", EditConditionHides))
	FVector Inertia;
	
	/** Resistance to rotation, slows down angular velocity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float Mass;

	/** Resistance to rotation, slows down angular velocity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float AngularDamping;

	/** Cap angular velocity to avoid unreasonable speeds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float MaxAngularVelocity;

	/** Additional torque due to frictional forces */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float FrictionTorque;

	/** Coefficient for drag forces that oppose rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float AngularDragCoefficient;

	/** Toggle to apply advanced damping effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	ETetherDampingModel DampingModel;
};

USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FAngularOutput
{
	GENERATED_BODY()

	FAngularOutput()
		: AngularVelocity(FVector::ZeroVector)
		, Inertia(FVector::OneVector)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector AngularVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Inertia;
};

USTRUCT(BlueprintType)
struct FTetherNarrowPhaseCollisionOutput
{
	GENERATED_BODY()

	FTetherNarrowPhaseCollisionOutput()
		: bHasCollision(false)
		, ContactPoint(FVector::ZeroVector)
		, PenetrationDepth(0.0f)
		, ShapeA(nullptr)
		, ShapeB(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	bool bHasCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector ContactPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float PenetrationDepth;

	TWeakObjectPtr<FTetherShape> ShapeA;
	TWeakObjectPtr<FTetherShape> ShapeB;
};

USTRUCT(BlueprintType)
struct FTetherBroadCollisionPair
{
	GENERATED_BODY()

	FTetherBroadCollisionPair()
		: FirstIndex(0)
		, SecondIndex(0)
	{}

	FTetherBroadCollisionPair(int32 InFirstIndex, int32 InSecondIndex)
		: FirstIndex(InFirstIndex)
		, SecondIndex(InSecondIndex)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	int32 FirstIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	int32 SecondIndex;
};

USTRUCT(BlueprintType)
struct FTetherBroadPhaseCollisionOutput
{
	GENERATED_BODY()

	FTetherBroadPhaseCollisionOutput()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	TArray<FTetherBroadCollisionPair> CollisionPairings;
};