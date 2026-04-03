using UnrealBuildTool;

public class spelet : ModuleRules
{
	public spelet(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UMG",
			"Slate",
			"SlateCore"
		});

		// PrivateDependencyModuleNames.AddRange(new string[] { "AITestSuite" });

		// (leave empty or add real modules if needed)
		PrivateDependencyModuleNames.AddRange(new string[] { });

	}
}