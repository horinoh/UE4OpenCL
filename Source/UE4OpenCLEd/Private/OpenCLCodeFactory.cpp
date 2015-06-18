// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCLEd.h"
#include "OpenCLCodeFactory.h"

#include "OpenCLCode.h"

#include "EditorFramework/AssetImportData.h"

UOpenCLCodeFactory::UOpenCLCodeFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UOpenCLCode::StaticClass();

	bCreateNew = false;
	bEditorImport = true;
	bText = true;
	Formats.Add(TEXT("cl;OpenCL Code"));
}

bool UOpenCLCodeFactory::DoesSupportClass(UClass* Class)
{
	return (Class == UOpenCLCode::StaticClass());
}

UClass* UOpenCLCodeFactory::ResolveSupportedClass()
{
	return UOpenCLCode::StaticClass();
}

UObject* UOpenCLCodeFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UOpenCLCode>(InParent, InName, Flags);
}

UObject* UOpenCLCodeFactory::FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn)
{
	auto NewAsset = NewObject<UOpenCLCode>(InParent, InName, Flags);
	if (nullptr != NewAsset)
	{
		NewAsset->Code = FString(Buffer);

		if (nullptr == NewAsset->AssetImportData)
		{
			NewAsset->AssetImportData = NewObject<UAssetImportData>(NewAsset);
		}
		if (nullptr != NewAsset->AssetImportData)
		{
			NewAsset->AssetImportData->SourceFilePath = FReimportManager::SanitizeImportFilename(CurrentFilename, NewAsset);
			NewAsset->AssetImportData->SourceFileTimestamp = IFileManager::Get().GetTimeStamp(*CurrentFilename).ToString();
			NewAsset->AssetImportData->bDirty = false;
		}
	}
	return NewAsset;
}

bool UOpenCLCodeFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	const auto Asset = Cast<UOpenCLCode>(Obj);
	if (nullptr != Asset)
	{
		if (nullptr != Asset->AssetImportData)
		{
			OutFilenames.Add(FReimportManager::ResolveImportFilename(Asset->AssetImportData->SourceFilePath, Asset));
			return true;
		}
	}
	return false;
}

void UOpenCLCodeFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths)
{
	const auto Asset = Cast<UOpenCLCode>(Obj);
	if (nullptr != Asset)
	{
		if (ensure(1 == NewReimportPaths.Num()))
		{
			Asset->AssetImportData->SourceFilePath = FReimportManager::ResolveImportFilename(NewReimportPaths[0], Asset);
		}
	}
}

EReimportResult::Type UOpenCLCodeFactory::Reimport(UObject* Obj)
{
	const auto Asset = Cast<UOpenCLCode>(Obj);
	if (nullptr != Asset)
	{
		const auto FileName = FReimportManager::ResolveImportFilename(Asset->AssetImportData->SourceFilePath, Asset);
		if (FileName.Len())
		{
			if (INDEX_NONE != IFileManager::Get().FileSize(*FileName))
			{
				if (UFactory::StaticImportObject(Asset->GetClass(), Asset->GetOuter(), *Asset->GetName(), RF_Public | RF_Standalone, *FileName, nullptr, this))
				{
					if (Asset->GetOuter())
					{
						Asset->GetOuter()->MarkPackageDirty();
					}
					else
					{
						Asset->MarkPackageDirty();
					}
					return EReimportResult::Succeeded;
				}
			}
		}
	}
	return EReimportResult::Failed;
}