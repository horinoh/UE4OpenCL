// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4OpenCL.h"
#include "OpenCLComponent.h"


// Sets default values for this component's properties
UOpenCLComponent::UOpenCLComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenCLComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

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
					}
				}
			}
		}
	}

	//!< OpenCL コードの読み込み、ビルド、実行 (ここでは最初のコンテキストを使用)
	//!< Load OpenCL code, build, execute (using first context here)
	if (0 < Contexts.Num() && 0 < DeviceIDs[0].Num())
	{
		cl_int ErrorCode;
		const auto CommandQueue = clCreateCommandQueue(Contexts[0], DeviceIDs[0][0], 0, &ErrorCode);
		if (CL_SUCCESS == ErrorCode)
		{
			const FString Code("kernel void main(__global float* buffer){const unsigned int id = get_global_id(0);buffer[id] = 9.0f - id;}");
			const char* Ansi = TCHAR_TO_ANSI(*Code);
			const size_t Length = Code.Len();
			const auto Program = clCreateProgramWithSource(Contexts[0], 1, &Ansi, &Length, &ErrorCode);
			if (CL_SUCCESS == ErrorCode)
			{
				if (CL_SUCCESS == clBuildProgram(Program, DeviceIDs[0].Num(), &DeviceIDs[0][0], nullptr, nullptr, nullptr))
				{
					const auto Kernel = clCreateKernel(Program, "main", &ErrorCode);
					if (CL_SUCCESS == ErrorCode) 
					{
						//!< GPU 上へメモリ確保
						//!< Allocate memory on GPU
						const auto Buffer = clCreateBuffer(Contexts[0], CL_MEM_READ_WRITE, sizeof(float) * 10, nullptr, &ErrorCode);
						if (CL_SUCCESS == ErrorCode)
						{
							//!< GPU へデータ転送 (コマンドキュー)
							//!< Send data to GPU (CommandQueue)
							const float in_data[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
							if (CL_SUCCESS == clEnqueueWriteBuffer(CommandQueue, Buffer, CL_TRUE, 0, sizeof(float) * 10, in_data, 0, nullptr, nullptr)) {
								//!< カーネル引数の設定
								//!< Arguments of kernel
								if (CL_SUCCESS == clSetKernelArg(Kernel, 0, sizeof(Buffer), &Buffer))
								{
									//!< カーネルの実行
									//!< Execute kernel
									const size_t GlobalWork = 10;
									const size_t LocalWork = 1;
									if (CL_SUCCESS == clEnqueueNDRangeKernel(CommandQueue, Kernel, 1, nullptr, &GlobalWork, &LocalWork, 0, nullptr, nullptr)) 
									{
										//!< 結果の取得
										//!< Get result
										float out_data[10];
										if (CL_SUCCESS == clEnqueueReadBuffer(CommandQueue, Buffer, CL_TRUE, 0, sizeof(float) * 10, &out_data, 0, nullptr, nullptr)) 
										{
											GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Done"));
										}
									}
								}
							}
						}
						clReleaseMemObject(Buffer);
					}
					clReleaseKernel(Kernel);
				}
			}
			clReleaseProgram(Program);
		}
		clReleaseCommandQueue(CommandQueue);
	}
}

void UOpenCLComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (auto i : Contexts)
	{
		clReleaseContext(i);
	}
}

// Called every frame
void UOpenCLComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

