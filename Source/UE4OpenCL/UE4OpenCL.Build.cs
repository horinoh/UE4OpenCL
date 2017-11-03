// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;
using Tools.DotNETCommon;

public class UE4OpenCL : ModuleRules
{
	public UE4OpenCL(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
															"RHI", "RenderCore", 
                                                            "UnrealEd", "AssetTools" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
		 	FileReference FR;
            UProjectInfo.TryGetProjectForTarget("UE4OpenCL", out FR);
            RulesAssembly RA = RulesCompiler.CreateProjectRulesAssembly(FR);
            var ModulePath = Path.GetDirectoryName(RA.GetModuleFileName(GetType().Name).CanonicalName);
			var ThirdPartyPath = Path.GetFullPath(Path.Combine(ModulePath, "..", "..", "ThirdParty"));

            //var Name = Path.Combine("CUDA", "v7.0");
            //var Name = Path.Combine("CUDA", "v7.5");
            var Name = Path.Combine("CUDA", "v8.0");
            var LibPath = Path.Combine(ThirdPartyPath, Name, "lib", "x64");
			PublicLibraryPaths.Add(LibPath);

			PublicAdditionalLibraries.Add(Path.Combine(LibPath, "OpenCL.lib"));
            PublicDelayLoadDLLs.Add("OpenCL64.dll");
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, Name, "include"));
		}
	}
}
