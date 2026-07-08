// AlexandriaLibrary.Build.cs
// The actor only needs core + engine modules; UStaticMeshComponent and
// UInstancedStaticMeshComponent both live in "Engine".

using UnrealBuildTool;

public class AlexandriaLibrary : ModuleRules
{
	public AlexandriaLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine"
		});

		// No extra modules required. If you later add UProceduralMeshComponent
		// geometry, add "ProceduralMeshComponent" here.
	}
}
