// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4OpenCLEditorTarget : TargetRules
{
	public UE4OpenCLEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange(new string[] { "UE4OpenCL", "UE4OpenCLEd" });
	}
}
