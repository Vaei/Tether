// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "TetherDeveloperSettings.generated.h"

class UTetherReplay;
class UTetherHashing;
class UTetherCollisionDetectionHandler;
class UTetherPhysicsSolver;

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
class TETHERPHYSICS_API UTetherDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** A map of gameplay tags to available hashing solutions for Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Hashing"))
	TMap<FGameplayTag, TSubclassOf<UTetherHashing>> HashingSystems;
	
	/** A map of gameplay tags to available solvers for Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Solver"))
	TMap<FGameplayTag, TSubclassOf<UTetherPhysicsSolver>> Solvers;
	
	/** A map of gameplay tags to available solvers for Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Solver"))
	TMap<FGameplayTag, TSubclassOf<UTetherReplay>> ReplaySystems;

	/** The class used to handle collisions between different shapes in the Tether physics system */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, NoClear, Category=Tether)
	TSubclassOf<UTetherCollisionDetectionHandler> CollisionDetectionHandler;
	
public:
	UTetherDeveloperSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Retrieves the default instance of UTetherDeveloperSettings */
	static const UTetherDeveloperSettings* Get() { return GetDefault<UTetherDeveloperSettings>(); }

	/** Retrieves the collision detection handler cast to the specified type */
	template<typename T>
	static T* GetCollisionDetectionHandler()
	{
		return Cast<T>(Get()->CollisionDetectionHandler);
	}

	/** Retrieves a hashing solution based on the provided gameplay tag and casts it to the specified type */
	template<typename T>
	static T* GetHashingSystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherHashing, T>(Tag, Get()->HashingSystems);
	}

	/** Retrieves a solver based on the provided gameplay tag and casts it to the specified type */
	template<typename T>
	static T* GetSolver(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherPhysicsSolver, T>(Tag, Get()->Solvers);
	}

	/** Retrieves a replay system based on the provided gameplay tag and casts it to the specified type */
	template<typename T>
	static T* GetReplaySystem(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherReplay, T>(Tag, Get()->ReplaySystems);
	}
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	/**
	 * Internal utility function to retrieve and cast a class instance from a map based on a gameplay tag.
	 * 
	 * @param Tag The gameplay tag used to lookup the class in the map.
	 * @param Map The map of gameplay tags to classes.
	 * @return A pointer to the instance of the class cast to the specified type, or nullptr if the cast fails.
	 */
	template<typename BaseType, typename T>
	static T* GetInternal(const FGameplayTag& Tag, const TMap<FGameplayTag, TSubclassOf<BaseType>>& Map)
	{
		if (const TSubclassOf<BaseType>* NewSolverClass = Map.Find(Tag))
		{
			if (BaseType* NewSolver = NewSolverClass->GetDefaultObject())
			{
				return Cast<T>(NewSolver);
			}
		}
		return nullptr;
	}
};
