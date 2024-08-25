// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "TetherDeveloperSettings.generated.h"

class UTetherShapeCollisionControl;
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

	/** All shapes available to Tether */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Tether, meta=(Categories="Tether.Shape"))
	TMap<FGameplayTag, TSubclassOf<UTetherPhysicsSolver>> Shapes;

	/** Class used to handle collisions between different shapes */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, NoClear, Category=Tether)
	TSubclassOf<UTetherShapeCollisionControl> ShapeCollisionControl;
	
public:
	UTetherDeveloperSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
