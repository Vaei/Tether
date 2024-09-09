// Copyright (c) Jared Taylor. All Rights Reserved.

#include "System/TetherMessaging.h"

#include "Debug/DebugDrawService.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TetherMessaging)

UWorld* FTetherDebugTextService::GetWorld() const
{
	if (WorldContext.IsValid())
	{
		return WorldContext->GetWorld();
	}
	return nullptr;
}

void FTetherDebugTextService::Initialize(UObject* InWorldContext, bool bDrawGame, bool bDrawEditor)
{
	// Note: These callbacks never provide a valid PlayerController
	
	WorldContext = InWorldContext;
	if (GetWorld())
	{
		// Draw in Game World (when possessed)
		if (bDrawGame && !GDebugDrawHandleGame.IsValid())
		{
			GDebugDrawHandleGame = UDebugDrawService::Register(TEXT("Game"), FDebugDrawDelegate::CreateLambda(
				[&](class UCanvas* Canvas, class APlayerController*)
			{
				if (GetWorld())
				{
					UTetherDrawing::ProcessText(&PendingDebugText, GetWorld(), Canvas);
				}
				else
				{
					Deinitialize();
				}
			}));
		}
		
#if WITH_EDITOR
		// Draw in Editor World (when not possessed)
		if (GIsEditor && bDrawEditor && !GDebugDrawHandleEditor.IsValid())
		{
			GDebugDrawHandleEditor = UDebugDrawService::Register(TEXT("Editor"), FDebugDrawDelegate::CreateLambda(
				[&](class UCanvas* Canvas, class APlayerController*)
			{
				if (GetWorld())
				{
					UTetherDrawing::ProcessText(&PendingDebugText, GetWorld(), Canvas);
				}
				else
				{
					Deinitialize();
				}
			}));
		}
#endif
	}
	else
	{
		// If the world is not valid, ensure that we clean up any existing debug services
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
