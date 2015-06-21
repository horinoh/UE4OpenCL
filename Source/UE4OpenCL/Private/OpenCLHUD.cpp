// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCL.h"
#include "OpenCLHUD.h"

#include "OpenCLComponent.h"
#include "OpenCLCode.h"

AOpenCLHUD::AOpenCLHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	OpenCLComp = ObjectInitializer.CreateDefaultSubobject<UOpenCLComponent>(this, TEXT("OpenCLComp"));

	static ConstructorHelpers::FObjectFinder<UOpenCLCode> Code(TEXT("OpenCLCode'/Game/OpenCLCode.OpenCLCode'"));
	if (nullptr != Code.Object)
	{
		OpenCLCode = Code.Object;
	}
}

void AOpenCLHUD::DrawHUD()
{	
	Super::DrawHUD();

	//!< カーネルの作成
	//!< Create kernel
	const auto Kernel = OpenCLComp->CreateKernel(*OpenCLCode, "main"); 
	{
		//!< コマンドキュー作成
		//!< Create command queue
		const auto CommandQueue = OpenCLComp->CreateCommandQueue();
		{
			const float InData[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
			
			//cl_image_format Format;
			//Format.image_channel_order = CL_RGBA;
			//Format.image_channel_data_type = CL_FLOAT;
			//const auto Image2D = OpenCLComp->CreateImage2D(&Format, 256, 256, 0);

			//!< バッファ作成
			//!< Create buffer
			const auto Buffer = OpenCLComp->CreateBuffer(sizeof(InData));
			{
				//!< GPU 上へメモリ確保
				//!< Allocate memory on GPU
				if (CL_SUCCESS == OpenCLComp->EnqueueWriteBuffer(CommandQueue, Buffer, sizeof(InData), InData))
				{
					//!< カーネル引数の設定
					//!< Arguments of kernel
					if (CL_SUCCESS == OpenCLComp->SetKernelArg(Kernel, 0, Buffer))
					{
						//!< カーネルの実行
						//!< Execute kernel
						if (CL_SUCCESS == OpenCLComp->EnqueueTask(CommandQueue, Kernel))
						{
							//!< 結果の取得
							//!< Get result
							float OutData[10];
							if (CL_SUCCESS == OpenCLComp->EnqueueReadBuffer(CommandQueue, Buffer, sizeof(OutData), OutData))
							{
								GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Execute OpenCL Done"));
							}
						}
					}
				}
			}
			clReleaseMemObject(Buffer);
			//clReleaseMemObject(Image2D);
		}
		clReleaseCommandQueue(CommandQueue);
	} 
	clReleaseKernel(Kernel);

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