// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherSettings.h"

#include "TetherDataAsset.h"
#include "TetherGameplayTags.h"
#include "Physics/Collision/TetherCollisionDetectionBroadPhase.h"
#include "Physics/Collision/TetherCollisionDetectionHandler.h"
#include "Physics/Collision/TetherCollisionDetectionNarrowPhase.h"
#include "Physics/Hashing/TetherHashingSpatial.h"
#include "Physics/Replay/TetherReplay.h"
#include "Physics/Solvers/Integration/TetherIntegrationSolverEuler.h"
#include "Physics/Solvers/Integration/TetherIntegrationSolverRK4.h"
#include "Physics/Solvers/Integration/TetherIntegrationSolverVerlet.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverLinear.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolverAngular.h"
#include "Shapes/TetherShape_AxisAlignedBoundingBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherSettings)

UTetherSettings::UTetherSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Default Hashing (Spatial)
	HashingSystems.Add({ FTetherGameplayTags::Tether_Hashing_Spatial.GetTag(), UTetherHashingSpatial::StaticClass() });

	// Default Collision and Detection
	BroadPhaseDetectionSystems.Add({ FTetherGameplayTags::Tether_Detection_BroadPhase.GetTag(), UTetherCollisionDetectionBroadPhase::StaticClass() });
	NarrowPhaseDetectionSystems.Add({ FTetherGameplayTags::Tether_Detection_NarrowPhase.GetTag(), UTetherCollisionDetectionNarrowPhase::StaticClass() });
	CollisionDetectionHandlers.Add({ FTetherGameplayTags::Tether_Detection_CollisionHandler.GetTag(), UTetherCollisionDetectionHandler::StaticClass() });

	// Default Activity State Handler
	ActivityStateHandlers.Add({ FTetherGameplayTags::Tether_ActivityState.GetTag(), UTetherActivityStateHandler::StaticClass() });
	
	// Default Physics Solvers
	LinearPhysicsSolvers.Add({ FTetherGameplayTags::Tether_Solver_Linear.GetTag(), UTetherPhysicsSolverLinear::StaticClass() });
	AngularPhysicsSolvers.Add({ FTetherGameplayTags::Tether_Solver_Angular.GetTag(), UTetherPhysicsSolverAngular::StaticClass() });
	
	// Default Integration Solvers
	IntegrationSolvers.Add({ FTetherGameplayTags::Tether_Solver_Integration_Euler.GetTag(), UTetherIntegrationSolverEuler::StaticClass() });
	IntegrationSolvers.Add({ FTetherGameplayTags::Tether_Solver_Integration_RK4.GetTag(), UTetherIntegrationSolverRK4::StaticClass() });
	IntegrationSolvers.Add({ FTetherGameplayTags::Tether_Solver_Integration_Verlet.GetTag(), UTetherIntegrationSolverVerlet::StaticClass() });

	// Default Replay System
	ReplaySystems.Add({ FTetherGameplayTags::Tether_Replay.GetTag(), UTetherReplay::StaticClass() });

#if WITH_EDITORONLY_DATA
	// Default Editor Subsystem Data Asset
	EditorSubsystemDataAsset = UTetherDataAsset::StaticClass()->GetDefaultObject();
#endif
}

#if WITH_EDITOR
void UTetherSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName().IsEqual(GET_MEMBER_NAME_CHECKED(ThisClass, EditorSubsystemDataAsset)))
	{
		if (EditorSubsystemDataAsset.IsNull())
		{
			// Default Editor Subsystem Data Asset
			EditorSubsystemDataAsset = UTetherDataAsset::StaticClass()->GetDefaultObject();
		}
	}
}
#endif