// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Lift_Off_Laetus : ModuleRules
{
	public Lift_Off_Laetus(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Slate", "SlateCore" });
    }
}
