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

	Texture2D = UTexture2D::CreateTransient(256, 256);
	if (nullptr != Texture2D)
	{
		Texture2D->UpdateResource();
	}
}

void AOpenCLHUD::DrawHUD()
{	
	Super::DrawHUD();

	if (nullptr != Texture2D)
	{
		//!< カーネルの作成
		//!< Create kernel
		const auto Kernel = OpenCLComp->CreateKernel(*OpenCLCode, "main");
		if (nullptr != Kernel)
		{
			//!< コマンドキュー作成
			//!< Create command queue
			const auto CommandQueue = OpenCLComp->CreateCommandQueue();
			if (nullptr != CommandQueue)
			{
				//!< イメージ作成
				//!< Create image
				const size_t Width = Texture2D->GetSizeX();
				const size_t Height = Texture2D->GetSizeY();
				cl_image_format Format;
				Format.image_channel_order = CL_BGRA;
				Format.image_channel_data_type = CL_UNSIGNED_INT8;
				const auto Buffer = OpenCLComp->CreateImage2D(&Format, Width, Height, 0);
				if (nullptr != Buffer)
				{
					//!< カーネル引数の設定
					//!< Arguments of kernel
					if (CL_SUCCESS == OpenCLComp->SetKernelArg(Kernel, 0, Buffer))
					{
						//!< カーネルの実行
						//!< Execute kernel
						const size_t GlobalWork[] = { Width, Height };
						if (CL_SUCCESS == OpenCLComp->EnqueueNDRangeKernel(CommandQueue, Kernel, ARRAY_COUNT(GlobalWork), GlobalWork))
						{
							Colors.Empty();
							Colors.SetNumUninitialized(Width * Height);
							const size_t Origin[] = { 0, 0, 0 };
							const size_t Region[] = { Width, Height, 1 };
							const size_t RowPitch = Width * sizeof(Colors[0]);
							const size_t SlicePitch = 0;

							//!< 結果の取得
							//!< Get result
							if (CL_SUCCESS == OpenCLComp->EnqueueReadImage(CommandQueue, Buffer, Origin, Region, RowPitch, SlicePitch, &Colors[0]))
							{
								//!< テクスチャを更新
								//!< Update texture
								ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(UpdateTexture2D,
									AOpenCLHUD*, This, this,
									UTexture2D*, Tex, Texture2D,
									{
										const auto Width = Tex->GetSizeX();
										const auto Height = Tex->GetSizeY();
										const auto Pitch = GPixelFormats[Tex->GetPixelFormat()].BlockBytes * Width;
										RHIUpdateTexture2D(Tex->Resource->TextureRHI->GetTexture2D(), 0, FUpdateTextureRegion2D(0, 0, 0, 0, Width, Height), Pitch, reinterpret_cast<const uint8*>(&This->Colors[0]));
									}
								);

								//!< テクスチャを表示
								//!< Display texture
								if (nullptr != Canvas)
								{
									const FVector2D ScreenSize(Canvas->SizeX, Canvas->SizeY);
									FCanvasTileItem Item(FVector2D::ZeroVector, Texture2D->Resource, ScreenSize, FLinearColor::White);
									Item.BlendMode = SE_BLEND_Opaque;
									Canvas->DrawItem(Item);
								}
							}
						}
					}
				}
				clReleaseMemObject(Buffer);
			}
			clReleaseCommandQueue(CommandQueue);
		}
		clReleaseKernel(Kernel);
	}
}