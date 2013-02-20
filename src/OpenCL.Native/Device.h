#pragma once

#include <CL/cl.h>

namespace OpenCL
{
using namespace System;

[Flags]
public enum class DeviceType : UInt32
{
	Cpu = CL_DEVICE_TYPE_CPU,
	Default = CL_DEVICE_TYPE_DEFAULT,
	Gpu = CL_DEVICE_TYPE_GPU,
	Acclerator = CL_DEVICE_TYPE_ACCELERATOR,
	All = CL_DEVICE_TYPE_ALL,
};

public ref class Device
{
internal:
	const cl_device_id _device_id;

internal:
	Device(cl_device_id id);
};

}