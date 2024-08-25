// Copyright (c) 2024 Jared Taylor. All Rights Reserved.


#include "TetherEditorSubsystem.h"

#include "EngineUtils.h"
#include "TetherEditorShapeActor.h"

void UTetherEditorSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	ShapeActors.Reset();
	
	for (TActorIterator<ATetherEditorShapeActor> It(GetWorld(), ATetherEditorShapeActor::StaticClass()); It; ++It)
	{
		ATetherEditorShapeActor* Actor = *It;
		ShapeActors.Add(Actor);
	}
}

void UTetherEditorSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (ATetherEditorShapeActor* ShapeActor : ShapeActors)
	{
		ShapeActor->TestCollisions(ShapeActors);
	}
}

TStatId UTetherEditorSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTetherEditorSubsystem, STATGROUP_Tickables);
}
