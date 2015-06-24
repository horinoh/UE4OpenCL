// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCL.h"
#include "AssetTypeActions_OpenCLCode.h"

#include "OpenCLCode.h"

UClass* FAssetTypeActions_OpenCLCode::GetSupportedClass() const
{
	return UOpenCLCode::StaticClass();
}

//void FAssetTypeActions_OpenCLCode::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
//{
//}