﻿// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "Physics/Collision/TetherCollisionDetectionBroadPhase.h"
#include "Physics/Collision/TetherCollisionDetectionNarrowPhase.h"
#include "Physics/Collision/TetherCollisionDetectionHandler.h"
#include "Physics/Hashing/TetherHashing.h"
#include "Physics/Replay/TetherReplay.h"
#include "Physics/Solvers/Physics/TetherPhysicsSolver.h"
#include "TetherSettings.generated.h"

/**
 * Configuration settings for the Tether physics system, accessible through Unreal Engine's developer settings.
 *
 * UTetherDeveloperSettings allows you to define and manage various components and configurations
 * related to the Tether physics system. These settings include available hashing solutions, physics solvers,
 * and the collision detection handler, all of which can be configured via the Unreal Engine editor.
 *
 * The settings are stored in the game's configuration file and can be edited in the editor or directly
 * in the configuration files. This class provides utility functions to easily retrieve the configured
 * instances of hashing solutions, solvers, and the collision detection handler at runtime.
 *
 * Key Features:
 * - Hashing Solutions: A map of gameplay tags to available hashing solution classes.
 * - Solvers: A map of gameplay tags to available physics solver classes.
 * - Collision Detection Handler: The class used to manage collisions between different shapes in the Tether physics system.
 */
UCLASS(Config=Game)
class TETHERPHYSICS_API UTetherSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** A map of gameplay tags to available hashing systems for Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Hashing"))
	TMap<FGameplayTag, TSubclassOf<UTetherHashing>> HashingSystems;

	/** A map of gameplay tags to available broad phase detection systems for Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Detection.BroadPhase"))
	TMap<FGameplayTag, TSubclassOf<UTetherCollisionDetectionBroadPhase>> BroadPhaseDetectionSystems;

	/** A map of gameplay tags to available narrow phase detection systems for Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Detection.NarrowPhase"))
	TMap<FGameplayTag, TSubclassOf<UTetherCollisionDetectionNarrowPhase>> NarrowPhaseDetectionSystems;
	
	/** A map of gameplay tags to available collision detection handlers for Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Detection.CollisionHandler"))
	TMap<FGameplayTag, TSubclassOf<UTetherCollisionDetectionHandler>> CollisionDetectionHandlers;

	/** A map of gameplay tags to available solvers for Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Solver"))
	TMap<FGameplayTag, TSubclassOf<UTetherPhysicsSolver>> Solvers;
	
	/** A map of gameplay tags to available solvers for Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Replay"))
	TMap<FGameplayTag, TSubclassOf<UTetherReplay>> ReplaySystems;

public:
	UTetherSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Retrieves the default instance of UTetherDeveloperSettings */
	static const UTetherSettings* Get() { return GetDefault<UTetherSettings>(); }

	/** Retrieves a broad phase detection system based on the provided gameplay tag and casts it to the specified type */
	template<typename T>
	static const T* GetBroadPhaseSystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherCollisionDetectionBroadPhase, T>(Tag, Get()->BroadPhaseDetectionSystems);
	}
	static const UTetherCollisionDetectionBroadPhase* GetBroadPhaseSystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherCollisionDetectionBroadPhase>(Tag, Get()->BroadPhaseDetectionSystems);
	}

	/** Retrieves a narrow phase detection system based on the provided gameplay tag and casts it to the specified type */
	template<typename T>
	static const T* GetNarrowPhaseSystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherCollisionDetectionNarrowPhase, T>(Tag, Get()->NarrowPhaseDetectionSystems);
	}
	static const UTetherCollisionDetectionNarrowPhase* GetNarrowPhaseSystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherCollisionDetectionNarrowPhase>(Tag, Get()->NarrowPhaseDetectionSystems);
	}

	/** Retrieves a narrow phase detection system based on the provided gameplay tag and casts it to the specified type */
	template<typename T>
	static const T* GetCollisionDetectionHandler(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherCollisionDetectionHandler, T>(Tag, Get()->CollisionDetectionHandlers);
	}
	static const UTetherCollisionDetectionHandler* GetCollisionDetectionHandler(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherCollisionDetectionHandler>(Tag, Get()->CollisionDetectionHandlers);
	}

	/** Retrieves a hashing solution based on the provided gameplay tag and casts it to the specified type */
	static const UTetherHashing* GetHashingSystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherHashing>(Tag, Get()->HashingSystems);
	}
	template<typename T>
	static const T* GetHashingSystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherHashing, T>(Tag, Get()->HashingSystems);
	}

	/** Retrieves a solver based on the provided gameplay tag and casts it to the specified type */
	template<typename T>
	static const T* GetSolver(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherPhysicsSolver, T>(Tag, Get()->Solvers);
	}
	static const UTetherPhysicsSolver* GetSolver(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherPhysicsSolver>(Tag, Get()->Solvers);
	}

	/** Retrieves a replay system based on the provided gameplay tag and casts it to the specified type */
	template<typename T>
	static const T* GetReplaySystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherReplay, T>(Tag, Get()->ReplaySystems);
	}
	static const UTetherReplay* GetReplaySystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherReplay>(Tag, Get()->ReplaySystems);
	}
	
protected:
	/**
	 * Internal utility function to retrieve and cast a class instance from a map based on a gameplay tag.
	 * 
	 * @param Tag The gameplay tag used to lookup the class in the map.
	 * @param Map The map of gameplay tags to classes.
	 * @return A pointer to the instance of the class cast to the specified type, or nullptr if the cast fails.
	 */
	template<typename BaseType>
	static const BaseType* GetInternal(const FGameplayTag& Tag, const TMap<FGameplayTag, TSubclassOf<BaseType>>& Map)
	{
		if (const TSubclassOf<BaseType>* BaseClass = Map.Find(Tag))
		{
			return BaseClass->GetDefaultObject();
		}
		return nullptr;
	}
	template<typename BaseType, typename T>
	static const T* GetInternal(const FGameplayTag& Tag, const TMap<FGameplayTag, TSubclassOf<BaseType>>& Map)
	{
		if (const TSubclassOf<BaseType>* BaseClass = Map.Find(Tag))
		{
			if (BaseType* DefaultBase = BaseClass->GetDefaultObject())
			{
				return Cast<T>(DefaultBase);
			}
		}
		return nullptr;
	}
};