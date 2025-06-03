// Author: Antonio Sidenko (Tonetfal), November 2024

using UnrealBuildTool;

public class TonetfalCommonUtilities : ModuleRules
{
	public TonetfalCommonUtilities(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreOnline",
				"DeveloperSettings",
				"GameplayTags",
				"UMG",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ApplicationCore",
				"CoreUObject",
				"Engine",
			}
		);
	}
}
