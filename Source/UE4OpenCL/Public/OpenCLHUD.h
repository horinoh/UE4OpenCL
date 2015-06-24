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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = OpenCL)
	class UOpenCLCode* OpenCLCode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Texture)
	UTexture2D* Texture2D;
	TArray<FColor> Colors;
};
