// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCL.h"

#include "AssetTypeActions_OpenCLCode.h"

class FOpenCLGameModuleImpl : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		AssetTypeActions = MakeShareable(new FAssetTypeActions_OpenCLCode());
		FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools")).Get().RegisterAssetTypeActions(AssetTypeActions.ToSharedRef());
	}
	virtual void ShutdownModule() override
	{
		if (AssetTypeActions.IsValid())
		{
			if (FModuleManager::Get().IsModuleLoaded(TEXT("AssetTools")))
			{
				FModuleManager::GetModuleChecked<FAssetToolsModule>(TEXT("AssetTools")).Get().UnregisterAssetTypeActions(AssetTypeActions.ToSharedRef());
			}
			AssetTypeActions.Reset();
		}
	}
	TSharedPtr<FAssetTypeActions_OpenCLCode> AssetTypeActions;
};

IMPLEMENT_PRIMARY_GAME_MODULE( FOpenCLGameModuleImpl, UE4OpenCL, "UE4OpenCL" );
