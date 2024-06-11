// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UWorldControl : ModuleRules
{
	public UWorldControl(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {

				// ... add other private include paths required here ...
			}
			);

	    if (Target.Type == TargetType.Editor)
	    {
	        PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd", });
	    }

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "UIds",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                //"CustomMeshComponent",
                //"ProceduralMeshComponent",
				// ... add private dependencies that you statically link with here ...	
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
