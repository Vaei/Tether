// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "TetherDeveloperSettings.generated.h"

class UTetherHashing;
class UTetherCollisionDetectionHandler;
class UTetherPhysicsSolver;
/**
 * 
 */
UCLASS(Config=Game)
class TETHERPHYSICS_API UTetherDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** All hashing solutions available to Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Hashing"))
	TMap<FGameplayTag, TSubclassOf<UTetherHashing>> Hashing;
	
	/** All solvers available to Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Solver"))
	TMap<FGameplayTag, TSubclassOf<UTetherPhysicsSolver>> Solvers;

	/** Class used to handle collisions between different shapes */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, NoClear, Category=Tether)
	TSubclassOf<UTetherCollisionDetectionHandler> CollisionDetectionHandler;
	
public:
	UTetherDeveloperSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const UTetherDeveloperSettings* Get() { return GetDefault<UTetherDeveloperSettings>(); }

	template<typename T>
	static T* GetCollisionDetectionHandler()
	{
		return Cast<T>(Get()->CollisionDetectionHandler);
	}
	
	template<typename T>
	static T* GetHashing(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherHashing, T>(Tag, Get()->Hashing);
	}
	
	template<typename T>
	static T* GetSolver(const FGameplayTag& Tag)
	{
		return GetInternal<UTetherPhysicsSolver, T>(Tag, Get()->Solvers);
	}
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
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
