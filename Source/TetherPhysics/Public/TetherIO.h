// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shapes/TetherShape.h"

#include "TetherIO.generated.h"


/** Damping model used for linear or angular motion. */
UENUM(BlueprintType)
enum class ETetherDampingModel : uint8
{
	SimpleLinear			UMETA(ToolTip="Subtracts a portion of angular velocity (a) each frame, based on damping coefficient (k) and time step (d): a *= (1 - k * d)"),
	ExponentialDecay		UMETA(ToolTip="Angular velocity (a) decreases over time with rate of decrease proportional to current velocity. More realistic in many physical systems, especially for simulating air resistance or other forms of damping that don't depend linearly on velocity: a *= exp(-k * d)"),
};

/**
 * Enum to define the strategy for sizing buckets in spatial hashing.
 *
 * This enumeration allows you to choose between automatic and fixed sizing strategies for buckets in the spatial grid.
 * Depending on the chosen strategy, the system will either automatically adjust the bucket size based on the largest shape
 * or use a fixed bucket size, with optional warnings when shapes do not fit within the bucket size.
 */
UENUM(BlueprintType)
enum class ETetherBucketSizingStrategy : uint8
{
	Automatic				UMETA(ToolTip="Automatically size the spatial grid by setting the bucket size to the size of the bounds of the largest shape"),
	AutomaticMax			UMETA(ToolTip="Only adjust the bucket size if it does not fit the largest shape, and optionally warn using: p.Tether.CheckSpatialHashingBounds 1"),
	Fixed					UMETA(ToolTip="Do not make any adjustments to the bucket size, and optionally warn using: p.Tether.CheckSpatialHashingBounds 1"),
};

/**
 * Determines the behavior of the replay system when handling multiple shapes.
 *
 * - Completion: This mode ensures that all shapes are checked, and the function will return true
 *   if at least one shape successfully replays its physics state. This is useful when you want to
 *   guarantee that all potential replays are evaluated, and the final result reflects whether any
 *   shape matched the given timestamp.
 *
 * - ShortCircuit: This mode causes the replay process to halt as soon as a successful replay is found.
 *   It returns true immediately after the first successful replay, making it more efficient in cases
 *   where finding the first match is sufficient. This mode can be particularly useful for optimizing
 *   performance when you do not need to evaluate all shapes.
 */
UENUM(BlueprintType)
enum class ETetherReplayMode : uint8
{
	Completion			UMETA(ToolTip="This mode ensures that all shapes are checked, and the function will return true if at least one shape successfully replays its physics state. This is useful when you want to guarantee that all potential replays are evaluated, and the final result reflects whether any shape matched the given timestamp."),
	ShortCircuit		UMETA(DisplayName="Short-Circuit", ToolTip="This mode causes the replay process to halt as soon as a successful replay is found. It returns true immediately after the first successful replay, making it more efficient in cases where finding the first match is sufficient. This mode can be particularly useful for optimizing performance when you do not need to evaluate all shapes."),
};

/**
 * Base struct for input/output operations in the Tether physics system.
 *
 * FTetherIO serves as a foundational structure designed to support polymorphism, allowing projects
 * to extend and customize data types for various purposes within the physics system. 
 * This includes creating custom solvers, hashing algorithms, and other physics-related components.
 *
 * By using FTetherIO as a base, derived structs can be seamlessly integrated into the Tether framework, 
 * enabling flexible and reusable code. This struct provides templated methods for setting and retrieving data, 
 * making it easier to manage different types of input/output operations.
 *
 * Key Features:
 * - Polymorphism: Supports inheritance, allowing for custom extensions specific to your project’s needs.
 * - Flexibility: Facilitates the creation of custom data structures for solvers, hashing, and other physics operations.
 * - Reusability: Provides a standardized way to handle various input/output data types within the Tether system.
 */
USTRUCT()
struct TETHERPHYSICS_API FTetherIO
{
	GENERATED_BODY()

	// SetDataIO should copy the data from the input StructData to the current instance
	template<typename StructType>
	void SetDataIO(const StructType& StructData)
	{
		*this = *reinterpret_cast<const FTetherIO*>(&StructData);
	}

    // GetDataIO returns a mutable pointer to the current instance cast to StructType
    template<typename StructType>
    StructType* GetDataIO()
    {
        return static_cast<StructType*>(this);
    }
    
    // GetDataIO returns a const pointer to the current instance cast to StructType
    template<typename StructType>
    const StructType* GetDataIO() const
    {
        return static_cast<const StructType*>(this);
    }
};

/**
 * Input data used in spatial hashing.
 *
 * This struct holds information required for performing spatial hashing, including bucket size,
 * origin, and references to the shapes being processed.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FSpatialHashingInput : public FTetherIO
{
	GENERATED_BODY()

	FSpatialHashingInput(const ETetherBucketSizingStrategy& InBucketSizeMode = ETetherBucketSizingStrategy::Fixed)
		: BucketSizeMode(InBucketSizeMode)
		, BucketSize(50.f)
		, OriginOffset(FVector::ZeroVector)
		, Shapes(nullptr)
	{}
    
	FSpatialHashingInput(const ETetherBucketSizingStrategy& InBucketSizeMode, const FVector& InBucketSize, const FVector& InOrigin, const TArray<FTetherShape*>* InShapes)
		: BucketSizeMode(InBucketSizeMode)
		, BucketSize(InBucketSize)
		, OriginOffset(InOrigin)
	{
		Shapes = InShapes;
	}

	/** Strategy for sizing buckets in spatial hashing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	ETetherBucketSizingStrategy BucketSizeMode;

	/** Size of each bucket in the spatial hash grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether, meta=(EditCondition="BucketSizeMode!=ETetherBucketSizeMode::Automatic", EditConditionHides))
	FVector BucketSize;

	/** Origin of the spatial hash grid */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector OriginOffset;

	/** Array of shapes to be hashed */
	const TArray<FTetherShape*>* Shapes;
};

/**
 * Output data produced by spatial hashing.
 *
 * This struct stores the results of spatial hashing, including the pairs of shapes
 * that should be tested for collisions and the spatial hash map itself.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FSpatialHashingOutput : public FTetherIO
{
	GENERATED_BODY()

	FSpatialHashingOutput()
		: BucketSize(FVector::ZeroVector)
	{}

	/** Size of each bucket in the spatial hash grid, after being computed according to FSpatialHashingInput::BucketSizeMode */
	FVector BucketSize;
	
	/** Pairs of shapes that should be tested for collisions */
	TArray<FTetherShapePair> ShapePairs;

	/** Spatial hash map storing shape indices by grid cell */
	TMap<FIntVector, TArray<int32>> SpatialHashMap;
};

/**
 * Input data for linear physics simulations.
 *
 * This struct includes properties for forces, mass, damping, and other factors
 * that influence the linear motion of an object.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FLinearInputSettings
{
	GENERATED_BODY()

	FLinearInputSettings()
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

/**
 * Input data for linear physics simulations.
 *
 * This struct includes properties for forces, mass, damping, and other factors
 * that influence the linear motion of an object.
 */
USTRUCT()
struct TETHERPHYSICS_API FLinearInput : public FTetherIO
{
	GENERATED_BODY()

	FLinearInput()
	{}

	TMap<const FTetherShape*, FLinearInputSettings> ShapeSettings;
};

/**
 * Input data for angular physics simulations.
 *
 * This struct includes properties for torques, mass, damping, shape, and other factors
 * that influence the angular motion of an object.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FAngularInputSettings : public FTetherIO
{
	GENERATED_BODY()

	FAngularInputSettings()
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

/**
 * Input data for angular physics simulations.
 *
 * This struct includes properties for torques, mass, damping, shape, and other factors
 * that influence the angular motion of an object.
 */
USTRUCT()
struct TETHERPHYSICS_API FAngularInput : public FTetherIO
{
	GENERATED_BODY()

	FAngularInput()
	{}

	TMap<const FTetherShape*, FAngularInputSettings> ShapeSettings;
};

/**
 * Output data for linear physics simulations.
 *
 * This struct includes properties such as the linear velocity of an object
 * after applying the linear physics calculations.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FLinearOutput : public FTetherIO
{
	GENERATED_BODY()

	FLinearOutput()
		: LinearVelocity(FVector::ZeroVector)
	{}

	/** Linear velocity resulting from the physics simulation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector LinearVelocity;
};

/**
 * Output data for angular physics simulations.
 *
 * This struct includes properties such as the angular velocity and inertia
 * of an object after applying the angular physics calculations.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FAngularOutput : public FTetherIO
{
	GENERATED_BODY()

	FAngularOutput()
		: AngularVelocity(FVector::ZeroVector)
		, Inertia(FVector::OneVector)
	{}

	/** Angular velocity resulting from the physics simulation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector AngularVelocity;

	/** Inertia of the object influencing angular motion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector Inertia;
};

/**
 * Output data of a narrow-phase collision check.
 *
 * This struct includes properties such as whether a collision occurred,
 * the contact point, and the penetration depth of the collision.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherNarrowPhaseCollisionOutput : public FTetherIO
{
	GENERATED_BODY()

	FTetherNarrowPhaseCollisionOutput()
		: bHasCollision(false)
		, ContactPoint(FVector::ZeroVector)
		, PenetrationDepth(0.f)
	{}

	/** Indicates whether a collision was detected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	bool bHasCollision;

	/** Contact point of the collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	FVector ContactPoint;

	/** Depth of penetration in the collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	float PenetrationDepth;
};

/**
 * Input data for broad-phase collision detection.
 *
 * This struct contains information needed for performing broad-phase collision detection,
 * including the array of shapes to be processed and the potential collision pairs identified
 * during spatial hashing.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherBroadPhaseCollisionInput : public FTetherIO
{
	GENERATED_BODY()

	FTetherBroadPhaseCollisionInput()
		: Shapes(nullptr)
		, PotentialCollisionPairings(nullptr)
	{}

	FTetherBroadPhaseCollisionInput(const TArray<FTetherShape*>* InShapes, const TArray<FTetherShapePair>* InPotentialPairs)
		: Shapes(InShapes)
		, PotentialCollisionPairings(InPotentialPairs)
	{}

	/** Array of shapes to be processed in the broad-phase collision detection */
	const TArray<FTetherShape*>* Shapes;

	/** Pairs of shapes that have been identified as potential collisions during spatial hashing */
	const TArray<FTetherShapePair>* PotentialCollisionPairings;
};

/**
 * Output data of a broad-phase collision detection.
 *
 * This struct includes an array of collision pairs that should be further tested
 * in the narrow-phase collision detection.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FTetherBroadPhaseCollisionOutput : public FTetherIO
{
	GENERATED_BODY()

	FTetherBroadPhaseCollisionOutput()
	{}

	/** Array of collision pairs detected in the broad-phase */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	TArray<FTetherShapePair> CollisionPairings;
};

/**
 * Single recorded frame in the physics simulation.
 *
 * This struct stores the linear and angular inputs for an object at a specific
 * time stamp, enabling accurate replay of the simulation.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FRecordedPhysicsFrame : public FTetherIO
{
	GENERATED_BODY()

	FRecordedPhysicsFrame()
		: TimeStamp(0.f)
	{}

	FRecordedPhysicsFrame(double InTimeStamp, const FLinearInput& InLinearInput, const FAngularInput& InAngularInput)
		: TimeStamp(InTimeStamp)
		, LinearInput(InLinearInput)
		, AngularInput(InAngularInput)
	{}

	/** The time at which this frame was recorded */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
	double TimeStamp;

	/** The linear input data at this frame */
	FLinearInput LinearInput;

	/** The angular input data at this frame */
	FAngularInput AngularInput;
};

/**
 * Recorded physics object in the simulation.
 *
 * This struct stores a pointer to the object being recorded, along with
 * a sequence of recorded frames that capture the object's state over time.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FRecordedPhysicsObject : public FTetherIO
{
	GENERATED_BODY()

	/** The shape being recorded */
	const FTetherShape* TetherShape;

	/** The sequence of recorded frames for this object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tether)
	TArray<FRecordedPhysicsFrame> RecordedFrames;

	/** 
	 * Adds a new frame to the recorded sequence 
	 * @param TimeStamp The time at which the frame is recorded
	 * @param LinearInput Pointer to the linear input data for the frame
	 * @param AngularInput Pointer to the angular input data for the frame
	 */
	void AddFrame(double TimeStamp, const FLinearInput* LinearInput, const FAngularInput* AngularInput)
	{
		RecordedFrames.Add(FRecordedPhysicsFrame(TimeStamp, *LinearInput, *AngularInput));
	}
};

/**
 * Entire recorded physics data for a simulation session.
 *
 * This struct stores the collection of all recorded objects in the session,
 * allowing for the complete replay of the simulation.
 */
USTRUCT(BlueprintType)
struct TETHERPHYSICS_API FRecordedPhysicsData : public FTetherIO
{
	GENERATED_BODY()

	/** The collection of all recorded objects in the session */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tether)
	TArray<FRecordedPhysicsObject> RecordedObjects;

	/**
	 * Find or create an object recording by pointer
	 * @param InTetherShape A pointer to the FTetherShape object
	 * @return A pointer to the recorded object data
	 */
	FRecordedPhysicsObject* FindOrCreateObjectRecording(const FTetherShape* InTetherShape)
	{
		for (FRecordedPhysicsObject& Obj : RecordedObjects)
		{
			if (Obj.TetherShape == InTetherShape)
			{
				return &Obj;
			}
		}

		FRecordedPhysicsObject NewObject;
		NewObject.TetherShape = InTetherShape;
		RecordedObjects.Add(NewObject);
		return &RecordedObjects.Last();
	}
};