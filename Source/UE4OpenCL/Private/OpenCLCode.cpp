// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCL.h"
#include "OpenCLCode.h"

void UOpenCLCode::PostInitProperties()
{
#if WITH_EDITORONLY_DATA
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		AssetImportData = NewObject<UAssetImportData>(this, TEXT("AssetImportData"));
	}
#endif
	Super::PostInitProperties();
}
