// Copyright (c) Jared Taylor. All Rights Reserved.

using UnrealBuildTool;

public class TetherTools : ModuleRules
{
    public TetherTools(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "AnimGraph",
                "AnimGraph",
                "BlueprintGraph",
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