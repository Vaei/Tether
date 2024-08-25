// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TetherActorComponent.generated.h"


/**
 * Handles tether physics in an actor component
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TETHER_API UTetherActorComponent : public UActorComponent
{
	GENERATED_BODY()
};
