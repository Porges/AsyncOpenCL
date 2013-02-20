#include "Platform.h"
#include "OpenCLException.h"

#include <msclr\marshal.h>
#include <vector>

using namespace OpenCL;
using namespace msclr::interop;

Platform::Platform(cl_platform_id id)
	: _platform_id(id)
	, _profile(gcnew Lazy<String^>(gcnew Func<String^>(this, &Platform::GetProfile)))
	, _version(gcnew Lazy<String^>(gcnew Func<String^>(this, &Platform::GetVersion)))
	, _name(gcnew Lazy<String^>(gcnew Func<String^>(this, &Platform::GetName)))
	, _vendor(gcnew Lazy<String^>(gcnew Func<String^>(this, &Platform::GetVendor)))
	, _extensions(gcnew Lazy<String^>(gcnew Func<String^>(this, &Platform::GetExtensions)))
{
}

String^ Platform::Get(cl_platform_info paramName)
{
	size_t requiredSize;
	auto errorCode = clGetPlatformInfo(_platform_id, paramName, 0, nullptr, &requiredSize);
	if (errorCode != CL_SUCCESS)
	{
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));
	}

	auto dest = gcnew array<unsigned char>(requiredSize);
	
	{
		pin_ptr<unsigned char> destPtr = &dest[0];
		errorCode = clGetPlatformInfo(_platform_id, paramName, requiredSize, destPtr, nullptr);
	}
	if (errorCode != CL_SUCCESS)
	{
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));
	}

	return System::Text::Encoding::ASCII->GetString(dest);
}

IEnumerable<Platform^>^ Platform::GetPlatforms()
{
	cl_uint numPlatforms;
	auto errCode = clGetPlatformIDs(0, nullptr, &numPlatforms);
	
	if (errCode != CL_SUCCESS)
	{
		throw gcnew OpenCLException(static_cast<ErrorCode>(errCode));
	}

	if (numPlatforms == 0)
		return System::Linq::Enumerable::Empty<Platform^>();

	auto arr = gcnew array<cl_platform_id>(numPlatforms);
	{
		pin_ptr<cl_platform_id> pinnedArr = &arr[0];
		errCode = clGetPlatformIDs(numPlatforms, pinnedArr, nullptr);
	}

	if (errCode != CL_SUCCESS)
	{
		throw gcnew OpenCLException(static_cast<ErrorCode>(errCode));
	}
	
	auto result = gcnew array<Platform^>(numPlatforms);
	for (auto i = 0; i < numPlatforms; ++i)
	{
		result[i] = gcnew Platform(arr[i]);
	}
	return result;
}

IEnumerable<Device^>^ Platform::GetDevices(DeviceType deviceType)
{
	cl_uint numDevices;
	auto errCode = clGetDeviceIDs(_platform_id, static_cast<cl_device_type>(deviceType), 0, nullptr, &numDevices);
	
	if (errCode != CL_SUCCESS)
	{
		throw gcnew OpenCLException(static_cast<ErrorCode>(errCode));
	}

	if (numDevices == 0)
		return System::Linq::Enumerable::Empty<Device^>();

	auto arr = gcnew array<cl_device_id>(numDevices);
	{
		pin_ptr<cl_device_id> pinnedArr = &arr[0];
		errCode = clGetDeviceIDs(_platform_id, static_cast<cl_device_type>(deviceType), numDevices, pinnedArr, nullptr);
	}

	if (errCode != CL_SUCCESS)
	{
		throw gcnew OpenCLException(static_cast<ErrorCode>(errCode));
	}
	
	auto result = gcnew array<Device^>(numDevices);
	for (auto i = 0; i < numDevices; ++i)
	{
		result[i] = gcnew Device(arr[i]);
	}
	return result;
}