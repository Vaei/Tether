﻿// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Misc/EngineVersionComparison.h"

#ifndef UE_5_04_OR_LATER
#if !UE_VERSION_OLDER_THAN(5, 4, 0)
#define UE_5_04_OR_LATER 1
#else
#define UE_5_04_OR_LATER 0
#endif
#endif