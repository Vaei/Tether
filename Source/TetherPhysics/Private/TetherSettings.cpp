﻿// Copyright (c) Jared Taylor. All Rights Reserved.


#include "TetherSettings.h"

#include "TetherGameplayTags.h"
#include "Physics/Collision/TetherCollisionDetectionBroadPhase.h"
#include "Physics/Collision/TetherCollisionDetectionHandler.h"
#include "Physics/Collision/TetherCollisionDetectionNarrowPhase.h"
#include "Physics/Hashing/TetherHashingSpatial.h"
#include "Physics/Replay/TetherReplay.h"
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
	
	// Default Solvers
	Solvers.Add({ FTetherGameplayTags::Tether_Solver_Physics_Linear.GetTag(), UTetherPhysicsSolverLinear::StaticClass() });
	Solvers.Add({ FTetherGameplayTags::Tether_Solver_Physics_Angular.GetTag(), UTetherPhysicsSolverAngular::StaticClass() });

	// Default Replay System
	ReplaySystems.Add({ FTetherGameplayTags::Tether_Replay.GetTag(), UTetherReplay::StaticClass() });
}