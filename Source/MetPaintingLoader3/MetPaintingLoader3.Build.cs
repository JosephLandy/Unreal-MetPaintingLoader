// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MetPaintingLoader3 : ModuleRules
{
	public MetPaintingLoader3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PrivateDependencyModuleNames.AddRange(new string[] {"UMG", "Blutility", "HTTP", "Json", "JsonUtilities", "PropertyEditor"});

		PrivateDependencyModuleNames.AddRange(new string[] { "ApplicationCore" }); // needed for copy-paste

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UnrealEd" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
