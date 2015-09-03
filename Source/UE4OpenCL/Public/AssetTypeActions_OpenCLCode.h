// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "EditorStyleSet.h"
#include "Settings/EditorLoadingSavingSettings.h"
#include "AssetTypeActions_Base.h"

/**
 * 
 */
class UE4OPENCL_API FAssetTypeActions_OpenCLCode : public FAssetTypeActions_Base
{
public:
	typedef FAssetTypeActions_Base Super;

	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "OpenCLCodeAssetTypeActions", "OpenCLCodeAsset"); }
	virtual FColor GetTypeColor() const override { return FColor::White; }
	virtual UClass* GetSupportedClass() const override;
	virtual uint32 GetCategories() override { return EAssetTypeCategories::Misc; }
	virtual bool IsImportedAsset() const override { return true; }

	//virtual bool HasActions(const TArray<UObject*>& InObjects) const override { return true; }
	//virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
	//void MyCustomAction(TArray<TWeakObjectPtr<class UMyCustomAsset>> Objects);

	//virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
};
