// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherDeveloperSettings.h"

#include "TetherGameplayTags.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverLinear.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverAngular.h"
#include "Shapes/TetherShapeCollisionControl.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherDeveloperSettings)

UTetherDeveloperSettings::UTetherDeveloperSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Default Solvers
	Solvers.Add({ FTetherGameplayTags::Tether_Solver_Physics_Linear.GetTag(), UTetherPhysicsSolverLinear::StaticClass() });
	Solvers.Add({ FTetherGameplayTags::Tether_Solver_Physics_Angular.GetTag(), UTetherPhysicsSolverAngular::StaticClass() });

	// Default Shapes
	Shapes.Add({ FTetherGameplayTags::Tether_Shape_AxisAlignedBoundingBox.GetTag(), UTetherShapeObject_AxisAlignedBoundingBox::StaticClass() });

	// Collision control
	ShapeCollisionControl = UTetherShapeCollisionControl::StaticClass();
}

#if WITH_EDITOR
void UTetherDeveloperSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName().IsEqual(GET_MEMBER_NAME_CHECKED(ThisClass, ShapeCollisionControl)))
	{
		// This doesn't prevent clearing it in DefaultGame.ini; at that point they're going to crash and its not worth
		// preventing that from happening
		if (!ShapeCollisionControl)
		{
			ShapeCollisionControl = UTetherShapeCollisionControl::StaticClass();
		}
	}
}
#endif