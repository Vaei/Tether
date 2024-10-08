// Copyright (c) Jared Taylor. All Rights Reserved.

using UnrealBuildTool;

public class Tether : ModuleRules
{
	public Tether(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"TetherPhysics",
				"GameplayTags",
				"GameplayTasks",
				"AnimGraphRuntime",
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
