// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

using UnrealBuildTool;

public class UROSControl : ModuleRules
{
	public UROSControl(ReadOnlyTargetRules Target) : base(Target)
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
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			  	"UROSBridge",
				"UConversions",
        		"UIds",
				"UWorldControl"
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
				"Networking",
				"Sockets",
				"PacketHandler",
				"libWebSockets",
				"OpenSSL",
				"zlib",
				"Json",
				"JsonUtilities"
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
