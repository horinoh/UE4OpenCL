// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EditorReimportHandler.h"

#include "Factories/Factory.h"
#include "OpenCLCodeFactory.generated.h"

/**
 * 
 */
UCLASS()
class UE4OPENCLED_API UOpenCLCodeFactory : public UFactory, public FReimportHandler
{
	GENERATED_BODY()
	
public:
	UOpenCLCodeFactory(const FObjectInitializer& ObjectInitializer);
	
	//!< UFactory
	virtual bool DoesSupportClass(UClass* Class) override;
	virtual UClass* ResolveSupportedClass() override;
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual UObject* FactoryCreateText(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn) override;

	//!< FReimportHandler
	virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
	virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
	virtual EReimportResult::Type Reimport(UObject* Obj) override;
};
