// Copyright 201    8, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

using UnrealBuildTool;
using System.IO;

public class URoboSim : ModuleRules
{
	public URoboSim(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        PublicDependencyModuleNames.AddRange(
					new string[]
					{
						"Core",
						"MeshDescription",
						"RenderCore",
						"Json",
						"XmlParser",
						"SeqLog",
            "UROSBridge",
          }
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
				{
					"CoreUObject",
					"Engine",
					"XmlParser",
				}
			);





		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
