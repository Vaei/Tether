// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "TetherDeveloperSettings.generated.h"

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
	/** All solvers available to Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Solver"))
	TMap<FGameplayTag, TSubclassOf<UTetherPhysicsSolver>> Solvers;

	/** Class used to handle collisions between different shapes */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, NoClear, Category=Tether)
	TSubclassOf<UTetherCollisionDetectionHandler> ShapeCollisionControl;
	
public:
	UTetherDeveloperSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static const UTetherDeveloperSettings* Get() { return GetDefault<UTetherDeveloperSettings>(); }
	
	template<typename T>
	static T* GetSolver(const FGameplayTag& Tag)
	{
		if (const TSubclassOf<UTetherPhysicsSolver>* NewSolverClass = Get()->Solvers.Find(Tag))
		{
			if (UTetherPhysicsSolver* NewSolver = NewSolverClass->GetDefaultObject())
			{
				return Cast<T>(NewSolver);
			}
		}
		return nullptr;
	}
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
