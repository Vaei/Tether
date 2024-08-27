// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Misc/EngineVersionComparison.h"

// Define a macro to check if the Unreal Engine version is 5.4.0 or later
#ifndef UE_5_04_OR_LATER
#if !UE_VERSION_OLDER_THAN(5, 4, 0)
#define UE_5_04_OR_LATER 1
#else
#define UE_5_04_OR_LATER 0
#endif
#endif