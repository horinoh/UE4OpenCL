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

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	TArray<cl_platform_id> PlatformIDs;
	TArray<TArray<cl_device_id>> DeviceIDs;
	TArray<cl_context> Contexts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Texture)
	class UTexture2D* Texture2D;
};
