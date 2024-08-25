// Copyright (c) 2024 Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TetherEditorShapeAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TETHEREDITOR_API UTetherEditorShapeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	const FAnimInstanceProxy& GetAnimInstanceProxy() const { return GetProxyOnGameThread<FAnimInstanceProxy>(); }
};
