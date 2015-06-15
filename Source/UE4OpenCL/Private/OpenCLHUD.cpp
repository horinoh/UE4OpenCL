// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCL.h"
#include "OpenCLHUD.h"

#include "OpenCLComponent.h"

AOpenCLHUD::AOpenCLHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OpenCLComp = ObjectInitializer.CreateDefaultSubobject<UOpenCLComponent>(this, TEXT("OpenCLComp"));
}

void AOpenCLHUD::DrawHUD()
{
	Super::DrawHUD();

//	if (nullptr != Canvas)
//	{
//		const FVector2D ScreenSize(Canvas->SizeX, Canvas->SizeY);
//#if 0
//		const auto Texture = OpenCLComp->Texture2D;
//		const FVector2D TextureSize(Texture->GetSurfaceWidth(), Texture->GetSurfaceHeight());
//		FCanvasTileItem Item((ScreenSize - TextureSize) * 0.5f, Texture->Resource, FLinearColor::White);
//#else
//		FCanvasTileItem Item(FVector2D::ZeroVector, OpenCLComp->Texture2D->Resource, ScreenSize, FLinearColor::White);
//#endif
//		Item.BlendMode = SE_BLEND_Opaque;
//		Canvas->DrawItem(Item);
//	}
}