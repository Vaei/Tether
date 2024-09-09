// Copyright (c) Jared Taylor. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TetherIO.h"
#include "UObject/Object.h"
#include "TetherActivityStateHandler.generated.h"

struct FTetherDebugText;

/**
 * 
 */
UCLASS(NotBlueprintable)
class TETHERPHYSICS_API UTetherActivityStateHandler : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * 
	 */
	virtual void PreSolveWake(const FTetherIO* InputData, const FTetherIO* LinearInputData, const FTetherIO* AngularInputData, float DeltaTime) const;
	
	virtual void PostSolveSleep(const FTetherIO* InputData, const FTetherIO* LinearInputData, const FTetherIO* AngularInputData, const FTetherIO* LinearOutputData, const FTetherIO* AngularOutputData, float DeltaTime) const;

	virtual void DrawDebug(const FTetherIO* InputData, TMap<const FTetherShape*, const FTransform*> ShapeTransforms,
		TArray<FTetherDebugText>* PendingDebugText = nullptr, float LifeTime = -1.f,
		FAnimInstanceProxy* Proxy = nullptr, const UWorld* World = nullptr, const FColor& WakeColor = FColor::White,
		const FColor& SleepColor = FColor::Black, bool bPersistentLines = false, float Thickness = 1.0f) const;
};
