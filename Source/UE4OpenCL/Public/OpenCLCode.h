// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "OpenCLCode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UE4OPENCL_API UOpenCLCode : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OpenCL)
	FString Code;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Instanced, Category = Reimport)
	class UAssetImportData* AssetImportData;
#endif
};
