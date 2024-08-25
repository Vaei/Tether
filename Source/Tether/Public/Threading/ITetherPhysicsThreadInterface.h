// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITetherPhysicsThreadInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTetherPhysicsThreadInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TETHER_API ITetherPhysicsThreadInterface
{
	GENERATED_BODY()

public:
	virtual bool IsPhysicsSimulationPaused() const { return true; }
	virtual void TickPhysics(float DeltaTime) PURE_VIRTUAL(,);
};
