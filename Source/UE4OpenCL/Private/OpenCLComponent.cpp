// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCL.h"
#include "OpenCLComponent.h"

#include "OpenCLCode.h"

// Sets default values for this component's properties
UOpenCLComponent::UOpenCLComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}

void UOpenCLComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	//!< プラットホームID、デバイスIDの列挙とコンテキストの作成
	//!< Enumerate platformIDs, deviceIDs and create context
	cl_uint PlatformNum;
	if (CL_SUCCESS == clGetPlatformIDs(0, nullptr, &PlatformNum) && 0 < PlatformNum)
	{
		PlatformIDs.AddUninitialized(PlatformNum);
		DeviceIDs.Reserve(PlatformNum);
		Contexts.Reserve(PlatformNum);
		if (CL_SUCCESS == clGetPlatformIDs(PlatformNum, &PlatformIDs[0], &PlatformNum))
		{
			for (auto i = 0; i < PlatformIDs.Num(); ++i)
			{
				cl_uint DeviceNum;
				if (CL_SUCCESS == clGetDeviceIDs(PlatformIDs[i], CL_DEVICE_TYPE_DEFAULT, 0, nullptr, &DeviceNum) && 0 < DeviceNum)
				{
					DeviceIDs.Add(TArray<cl_device_id>());
					DeviceIDs.Last().AddUninitialized(DeviceNum);
					if (CL_SUCCESS == clGetDeviceIDs(PlatformIDs[i], CL_DEVICE_TYPE_DEFAULT, DeviceNum, &DeviceIDs.Last()[0], &DeviceNum))
					{
						cl_int ErrorCode;
						Contexts.Add(clCreateContext(nullptr, DeviceNum, &DeviceIDs[i][0], nullptr, nullptr, &ErrorCode));
						check(CL_SUCCESS == ErrorCode);
						if (CL_SUCCESS == ErrorCode)
						{
						}
					}
				}
			}
		}
	}
}
void UOpenCLComponent::OnComponentDestroyed()
{
	Super::OnComponentDestroyed();

	for (auto i : Contexts)
	{
		clReleaseContext(i);
	}
}

// Called when the game starts
void UOpenCLComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UOpenCLComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

cl_kernel UOpenCLComponent::CreateKernel(const class UOpenCLCode& CodeAsset, const FString KernelName)
{
	cl_kernel Kernel = nullptr;
	//!< OpenCL コードの読み込み、ビルド(ここでは最初のコンテキストを使用)
	//!< Load OpenCL code, build(using first context here)
	if (0 < Contexts.Num() && 0 < DeviceIDs[0].Num())
	{
		cl_int ErrorCode;

		const char* AnsiCode = TCHAR_TO_ANSI(*CodeAsset.Code);
		const size_t Length = CodeAsset.Code.Len();
		const auto Program = clCreateProgramWithSource(Contexts[0], 1, &AnsiCode, &Length, &ErrorCode);
		if (CL_SUCCESS == ErrorCode)
		{
			if (CL_SUCCESS == clBuildProgram(Program, DeviceIDs[0].Num(), &DeviceIDs[0][0], nullptr, nullptr, nullptr))
			{
				Kernel = clCreateKernel(Program, TCHAR_TO_ANSI(*KernelName), &ErrorCode);
				if (CL_SUCCESS == ErrorCode)
				{
				}
			}
			clReleaseProgram(Program);
		}
	}
	return Kernel;
}

cl_command_queue UOpenCLComponent::CreateCommandQueue()
{
	cl_command_queue CommandQueue = nullptr;
	if (0 < Contexts.Num() && 0 < DeviceIDs[0].Num())
	{
		cl_int ErrorCode;
	
		//!< コマンドキュー作成
		//!< Create command queue
		CommandQueue = clCreateCommandQueue(Contexts[0], DeviceIDs[0][0], 0, &ErrorCode);
		if (CL_SUCCESS == ErrorCode)
		{
		}
	}
	return CommandQueue;
}

cl_mem UOpenCLComponent::CreateBuffer(const size_t Size, const cl_mem_flags MemFlags)
{
	cl_mem Buffer = nullptr;
	if (0 < Contexts.Num())
	{
		cl_int ErrorCode;

		//!< GPU 上へメモリ確保
		//!< Allocate memory on GPU
		Buffer = clCreateBuffer(Contexts[0], MemFlags, Size, nullptr, &ErrorCode);
		if (CL_SUCCESS == ErrorCode)
		{
		}
	}
	return Buffer;
}
cl_mem UOpenCLComponent::CreateImage2D(const cl_image_format* Format, const size_t Width, const size_t Height, const size_t Pitch, const cl_mem_flags MemFlags)
{
	cl_mem Buffer = nullptr;
	if (0 < Contexts.Num())
	{
		cl_int ErrorCode;

		clCreateImage2D(Contexts[0], MemFlags, Format, Width, Height, Pitch, nullptr, &ErrorCode);
		if (CL_SUCCESS == ErrorCode)
		{
		}
	}
	return Buffer;
}

cl_int UOpenCLComponent::EnqueueWriteBuffer(const cl_command_queue CommandQueue, const cl_mem Buffer, const size_t Size, const void* InData, const size_t Offset, const cl_bool Blocking)
{
	//!< GPU へデータ転送 (コマンドキュー)
	//!< Send data to GPU (CommandQueue)
	return clEnqueueWriteBuffer(CommandQueue, Buffer, Blocking, Offset, Size, InData, 0, nullptr, nullptr);
}
cl_int UOpenCLComponent::EnqueueReadBuffer(const cl_command_queue CommandQueue, const cl_mem Buffer, const size_t Size, void* OutData, const size_t Offset, const cl_bool Blocking)
{
	//!< 結果の取得
	//!< Get result
	return clEnqueueReadBuffer(CommandQueue, Buffer, Blocking, Offset, Size, OutData, 0, nullptr, nullptr);
}
cl_int UOpenCLComponent::EnqueueWriteImage(const cl_command_queue CommandQueue, const cl_mem Buffer, const size_t* Origin, const size_t* Region, const size_t RowPitch, const size_t SlicePitch, const void* InData, const cl_bool Blocking)
{
	return clEnqueueWriteImage(CommandQueue, Buffer, Blocking, Origin, Region, RowPitch, SlicePitch, InData, 0, nullptr, nullptr);
}
cl_int UOpenCLComponent::EnqueueReadImage(const cl_command_queue CommandQueue, const cl_mem Buffer, const size_t* Origin, const size_t* Region, const size_t RowPitch, const size_t SlicePitch, void* OutData, const cl_bool Blocking)
{
	return clEnqueueReadImage(CommandQueue, Buffer, Blocking, Origin, Region, RowPitch, SlicePitch, OutData, 0, nullptr, nullptr);
}

cl_int UOpenCLComponent::SetKernelArg(const cl_kernel Kernel, const cl_uint Index, const cl_mem Buffer)
{
	//!< カーネル引数の設定
	//!< Arguments of kernel
	return clSetKernelArg(Kernel, Index, sizeof(Buffer), &Buffer);
}
cl_int UOpenCLComponent::EnqueueTask(const cl_command_queue CommandQueue, const cl_kernel Kernel)
{
	//!< カーネルの実行
	//!< Execute kernel
#if 0
	return clEnqueueTask(CommandQueue, Kernel, 0, nullptr, nullptr);
#else
	const cl_uint WorkDim = 1;
	const size_t GlobalWorkOffset = 0;
	const size_t GlobalWork = 10;
	const size_t LocalWork = 1;
	return clEnqueueNDRangeKernel(CommandQueue, Kernel, WorkDim, &GlobalWorkOffset, &GlobalWork, &LocalWork, 0, nullptr, nullptr);
#endif
}