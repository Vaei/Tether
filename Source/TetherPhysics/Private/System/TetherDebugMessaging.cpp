// Copyright (c) Jared Taylor. All Rights Reserved.

#include "System/TetherDebugMessaging.h"

#include "Debug/DebugDrawService.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherDebugMessaging)

void FTetherDebugTextService::Initialize(UObject* InWorldContext, bool bDrawGame, bool bDrawEditor)
{
	WorldContext = InWorldContext;
	if (GetWorld())
	{
		// Draw in Game World (when possessed)
		if (bDrawGame && !GDebugDrawHandleGame.IsValid())
		{
			GDebugDrawHandleGame = UDebugDrawService::Register(TEXT("Game"), FDebugDrawDelegate::CreateLambda(
				[&](class UCanvas* Canvas, class APlayerController*)
			{
				UTetherStatics::ProcessText(&PendingDebugText, GetWorld(), Canvas);
			}));
		}
		
#if WITH_EDITOR
		// Draw in Editor World (when not possessed)
		if (GIsEditor && bDrawEditor && !GDebugDrawHandleEditor.IsValid())
		{
			GDebugDrawHandleEditor = UDebugDrawService::Register(TEXT("Editor"), FDebugDrawDelegate::CreateLambda(
				[&](class UCanvas* Canvas, class APlayerController*)
			{
				UTetherStatics::ProcessText(&PendingDebugText, GetWorld(), Canvas);
			}));
		}
#endif
	}
	else
	{
		Deinitialize();
	}
}

void FTetherDebugTextService::Deinitialize()
{
	if (GDebugDrawHandleGame.IsValid())
	{
		UDebugDrawService::Unregister(GDebugDrawHandleGame);
		GDebugDrawHandleGame.Reset();
	}

#if WITH_EDITOR
	if (GIsEditor && GDebugDrawHandleEditor.IsValid())
	{
		UDebugDrawService::Unregister(GDebugDrawHandleEditor);
		GDebugDrawHandleEditor.Reset();
	}
#endif
}
