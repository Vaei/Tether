// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "TetherPhysicsSolver.h"
#include "TetherPhysicsSolverAngular.generated.h"

/**
 * 
 */
UCLASS()
class TETHERPHYSICS_API UTetherPhysicsSolverAngular : public UTetherPhysicsSolver
{
	GENERATED_BODY()

public:
	void Solve(const FAngularInput& Input, FAngularOutput& Output, const FTransform& Transform, float DeltaTime, float Radius) const;
	void Solve(const FAngularInput& Input, FAngularOutput& Output, const FTransform& Transform, float DeltaTime, float HalfHeight, float Radius) const;
	virtual void Solve(const FAngularInput& Input, FAngularOutput& Output, const FTransform& Transform, float DeltaTime, const FVector& BoxExtent) const;
};
