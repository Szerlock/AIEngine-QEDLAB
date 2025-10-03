// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AIEngine : ModuleRules
{
	public AIEngine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AIEngine",
			 "AIEngine/Variant_Platforming",
			 "AIEngine/Variant_Platforming/Animation",
			 "AIEngine/Variant_Combat",
			 "AIEngine/Variant_Combat/AI",
			 "AIEngine/Variant_Combat/Animation",
			 "AIEngine/Variant_Combat/Gameplay",
			 "AIEngine/Variant_Combat/Interfaces",
			 "AIEngine/Variant_Combat/UI",
			 "AIEngine/Variant_SideScrolling",
			 "AIEngine/Variant_SideScrolling/AI",
			 "AIEngine/Variant_SideScrolling/Gameplay",
			 "AIEngine/Variant_SideScrolling/Interfaces",
			 "AIEngine/Variant_SideScrolling/UI",
			"AIEngine/AStarPathFinding",
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
