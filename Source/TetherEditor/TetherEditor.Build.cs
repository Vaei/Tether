// Copyright (c) Jared Taylor. All Rights Reserved.

using UnrealBuildTool;

public class TetherEditor : ModuleRules
{
    public TetherEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "TetherPhysics",
                "GameplayTags",
                "Tether",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
            }
        );
    }
}