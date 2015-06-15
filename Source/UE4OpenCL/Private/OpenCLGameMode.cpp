// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCL.h"
#include "OpenCLGameMode.h"

#include "OpenCLHUD.h"

AOpenCLGameMode::AOpenCLGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HUDClass = AOpenCLHUD::StaticClass();
}


