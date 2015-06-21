// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CL/cl.h>

#include "Components/ActorComponent.h"
#include "OpenCLComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4OPENCL_API UOpenCLComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenCLComponent();

	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual cl_kernel CreateKernel(const class UOpenCLCode& CodeAsset, const FString KernelName);
	virtual cl_command_queue CreateCommandQueue();

	virtual cl_mem CreateBuffer(const size_t Size, const cl_mem_flags MemFlags = CL_MEM_READ_WRITE);
	virtual cl_mem CreateImage2D(const cl_image_format* Format, const size_t Width, const size_t Height, const size_t Pitch, const cl_mem_flags MemFlags = CL_MEM_WRITE_ONLY);
	
	virtual cl_int EnqueueWriteBuffer(const cl_command_queue CommandQueue, const cl_mem Buffer, const size_t Size, const void* InData, const size_t Offset = 0, const cl_bool Blocking = CL_TRUE);
	virtual cl_int EnqueueReadBuffer(const cl_command_queue CommandQueue, const cl_mem Buffer, const size_t Size, void* OutData, const size_t Offset = 0, const cl_bool Blocking = CL_TRUE);
	virtual cl_int EnqueueWriteImage(const cl_command_queue CommandQueue, const cl_mem Buffer, const size_t* Origin, const size_t* Region, const size_t RowPitch, const size_t SlicePitch, const void* InData, const cl_bool Blocking = CL_TRUE);
	virtual cl_int EnqueueReadImage(const cl_command_queue CommandQueue, const cl_mem Buffer, const size_t* Origin, const size_t* Region, const size_t RowPitch, const size_t SlicePitch, void* OutData, const cl_bool Blocking = CL_TRUE);

	virtual cl_int SetKernelArg(const cl_kernel Kernel, const cl_uint Index, const cl_mem Buffer);
	virtual cl_int EnqueueTask(const cl_command_queue CommandQueue, const cl_kernel Kernel);
	virtual cl_int EnqueueNDRangeKernel(const cl_command_queue CommandQueue, const cl_kernel Kernel, const cl_uint WorkDim, const size_t GlobalWork, const size_t LocalWork, const size_t GlobalWorkOffset = 0);

	TArray<cl_platform_id> PlatformIDs;
	TArray<TArray<cl_device_id>> DeviceIDs;
	TArray<cl_context> Contexts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Texture)
	class UTexture2D* Texture2D;
};
