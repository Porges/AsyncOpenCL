#pragma once

#include "Context.h"
#include "Device.h"

#include <CL/cl.h>

namespace OpenCL
{
using namespace System;
using namespace System::Collections::Generic;

public ref class Platform
{
internal:
	const cl_platform_id _platform_id;

private:
	Lazy<String^>^ _profile;
	Lazy<String^>^ _version;
	Lazy<String^>^ _name;
	Lazy<String^>^ _vendor;
	Lazy<String^>^ _extensions;

	Platform(cl_platform_id id);
	
	String^ GetProfile() { return Get(CL_PLATFORM_PROFILE); }
	String^ GetVersion() { return Get(CL_PLATFORM_VERSION); }
	String^ GetName() { return Get(CL_PLATFORM_NAME); }
	String^ GetVendor() { return Get(CL_PLATFORM_VENDOR); }
	String^ GetExtensions() { return Get(CL_PLATFORM_EXTENSIONS); }

	String^ Get(cl_platform_info paramName);

public:
	static IEnumerable<Platform^>^ GetPlatforms();

	IEnumerable<Device^>^ GetDevices(DeviceType deviceType);
	
	property String^ Profile
	{
		String^ get()
		{
			return _profile->Value;
		}
	}

	property String^ Version
	{
		String^ get()
		{
			return _version->Value;
		}
	}

	property String^ Name
	{
		String^ get()
		{
			return _name->Value;
		}
	}

	property String^ Vendor
	{
		String^ get()
		{
			return _vendor->Value;
		}
	}

	property String^ Extensions
	{
		String^ get()
		{
			return _extensions->Value;
		}
	}
};

}