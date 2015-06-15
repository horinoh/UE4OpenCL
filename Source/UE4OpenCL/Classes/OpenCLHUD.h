// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "OpenCLHUD.generated.h"

/**
 * 
 */
UCLASS()
class UE4OPENCL_API AOpenCLHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AOpenCLHUD(const FObjectInitializer& ObjectInitializer);

	virtual void DrawHUD() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = OpenCL)
	class UOpenCLComponent* OpenCLComp;
};
