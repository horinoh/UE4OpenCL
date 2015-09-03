// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCLEd.h"
#include "OpenCLCodeFactory.h"

#include "Editor.h"
#include "EditorFramework/AssetImportData.h"

#include "OpenCLCode.h"

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
	FEditorDelegates::OnAssetPreImport.Broadcast(this, InClass, InParent, InName, Type);

	auto NewAsset = NewObject<UOpenCLCode>(InParent, InName, Flags);
	if (nullptr != NewAsset)
	{
		NewAsset->Code = FString(Buffer);
	
		if (nullptr != NewAsset->AssetImportData)
		{
			NewAsset->AssetImportData->Update(CurrentFilename);
		}
	}

	FEditorDelegates::OnAssetPostImport.Broadcast(this, NewAsset);

	return NewAsset;
}

bool UOpenCLCodeFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames)
{
	const auto Asset = Cast<UOpenCLCode>(Obj);
	if (nullptr != Asset)
	{
		if (nullptr != Asset->AssetImportData)
		{
			//!< AssetImportData->SourceData.SourceFiles ���Q�Ƃ��ĕԂ��Ă���
			Asset->AssetImportData->ExtractFilenames(OutFilenames);
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
		if (nullptr != Asset->AssetImportData)
		{
			if (ensure(1 == NewReimportPaths.Num()))
			{
				//!< AssetImportData->SourceData.SourceFiles[0] �փZ�b�g���Ă���A�^�C���X�^���v�͍X�V���Ȃ�
				Asset->AssetImportData->UpdateFilenameOnly(NewReimportPaths[0]);
			}
		}
	}
}

EReimportResult::Type UOpenCLCodeFactory::Reimport(UObject* Obj)
{
	const auto Asset = Cast<UOpenCLCode>(Obj);
	if (nullptr != Asset)
	{
		if (nullptr != Asset->AssetImportData)
		{
			//!< AssetImportData->SourceData.SourceFiles[0] ���Q�Ƃ��ĕԂ��Ă���
			const auto Filename = Asset->AssetImportData->GetFirstFilename();
			if (Filename.Len())
			{
				if (INDEX_NONE != IFileManager::Get().FileSize(*Filename))
				{
					if (UFactory::StaticImportObject(Asset->GetClass(), Asset->GetOuter(), *Asset->GetName(), RF_Public | RF_Standalone, *Filename, nullptr, this))
					{
						//!< �w�肳�ꂽ�t�@�C����p���čX�V����
						Asset->AssetImportData->Update(Filename);

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
	}
	return EReimportResult::Failed;
}