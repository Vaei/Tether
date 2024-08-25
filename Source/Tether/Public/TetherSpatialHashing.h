// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Shapes/TetherShape.h"

#include "TetherSpatialHashing.generated.h"

USTRUCT(BlueprintType)
struct FTetherSpatialHashingInput
{
    GENERATED_BODY()
    
    FTetherSpatialHashingInput()
        : BucketSize(50.f)
    {}
    
    FTetherSpatialHashingInput(const FVector& InBucketSize)
        : BucketSize(InBucketSize)
    {}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Tether)
    FVector BucketSize;
};

struct FTetherShapePair
{
    int32 ShapeIndexA;  // Index of the first shape in the pair
    int32 ShapeIndexB;  // Index of the second shape in the pair

    // Constructor to initialize the pair
    FTetherShapePair(int32 InShapeIndexA, int32 InShapeIndexB)
        : ShapeIndexA(InShapeIndexA), ShapeIndexB(InShapeIndexB) {}
    
    // Method to check equality between two shape pairs
    bool operator==(const FTetherShapePair& Other) const
    {
        return (ShapeIndexA == Other.ShapeIndexA && ShapeIndexB == Other.ShapeIndexB) ||
               (ShapeIndexA == Other.ShapeIndexB && ShapeIndexB == Other.ShapeIndexA);
    }
};

struct FTetherSpatialHashing
{
    FTetherSpatialHashingInput Input;

    TArray<FTetherShapePair> ShapePairs;
    
    TMap<FIntVector, TArray<int32>> SpatialHashMap;

    FVector Origin;

    FTetherSpatialHashing()
        : Origin(FVector::ZeroVector)
    {}
    
    FTetherSpatialHashing(const FTetherSpatialHashingInput& Input, const TArray<FTetherShape>& Shapes, const FVector& InOrigin)
        : Input(Input)
        , Origin(InOrigin)
    {
        GenerateShapePairs(Shapes);
    }

    // Function to compute the spatial hash key for a given shape
    FIntVector ComputeSpatialHashKey(const FTetherShape& Shape) const
    {
        FVector Position = Shape.GetCenter(); // Assuming GetCenter() returns the shape's position
        return FIntVector(
            FMath::FloorToInt((Position.X - Origin.X) / Input.BucketSize.X),
            FMath::FloorToInt((Position.Y - Origin.Y) / Input.BucketSize.Y),
            FMath::FloorToInt((Position.Z - Origin.Z) / Input.BucketSize.Z)
        );
    }

    // Function to add shapes to the spatial hash map
    void AddShapeToSpatialHash(int32 ShapeIndex, const FTetherShape& Shape)
    {
        FIntVector HashKey = ComputeSpatialHashKey(Shape);
        SpatialHashMap.FindOrAdd(HashKey).Add(ShapeIndex);
    }

    static bool AreBucketsAdjacent(int32 BucketA, int32 BucketB)
    {
        // Implement logic to determine if two buckets are adjacent
        // This could involve a simple comparison of bucket indices or more complex adjacency logic

        // DEVNOTE: This is Simple 1D Adjacency, but I'm not convinced 3D would actually perform better
        return FMath::Abs(BucketA - BucketB) <= 1;
    }

    static bool IsInSameOrAdjacentBucket(const FTetherShape& ShapeA, const FTetherShape& ShapeB)
    {
        // Implement logic to determine if two shapes are in the same or adjacent buckets
        // This could involve checking spatial hash values or bucket coordinates

        return ShapeA.Bucket == ShapeB.Bucket || AreBucketsAdjacent(ShapeA.Bucket, ShapeB.Bucket);
    }

    /** Generate shape pairs based on spatial hashing and efficiency rating */
    void GenerateShapePairs(const TArray<FTetherShape>& Shapes)
    {
        // Clear previous pairs and hash map
        ShapePairs.Empty();
        SpatialHashMap.Empty();

        // Add all shapes to the spatial hash map
        for (int32 i = 0; i < Shapes.Num(); i++)
        {
            AddShapeToSpatialHash(i, Shapes[i]);
        }

        // Generate pairs based on spatial proximity and efficiency rating
        for (const auto& HashEntry : SpatialHashMap)
        {
            const TArray<int32>& Indices = HashEntry.Value;

            for (int32 i = 0; i < Indices.Num(); i++)
            {
                for (int32 j = i + 1; j < Indices.Num(); j++)
                {
                    int32 IndexA = Indices[i];
                    int32 IndexB = Indices[j];

                    const FTetherShape& ShapeA = Shapes[IndexA];
                    const FTetherShape& ShapeB = Shapes[IndexB];

                    // Check if shapes are in the same or adjacent buckets
                    if (IsInSameOrAdjacentBucket(ShapeA, ShapeB))
                    {
                        // Determine which shape should perform the evaluation based on EfficiencyRating
                        if (ShapeA.EfficiencyRating >= ShapeB.EfficiencyRating)
                        {
                            ShapePairs.Add(FTetherShapePair(IndexA, IndexB));
                        }
                        else
                        {
                            ShapePairs.Add(FTetherShapePair(IndexB, IndexA));
                        }
                    }
                }
            }
        }
    }

    static void DrawDebugBucket(FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World, const FIntVector& BucketIndex, const FVector& BucketSize, const FColor& Color = FColor::Green, bool bPersistentLines = false, float LifeTime = -1.0f, float Thickness = 1.0f);
    void DrawDebugSpatialGrid(FAnimInstanceProxy* AnimInstanceProxy, const UWorld* World, bool bDrawAllBuckets = true, const FColor& Color = FColor::Green, bool bPersistentLines = false, float LifeTime = -1.0f, float Thickness = 1.0f) const;
};
