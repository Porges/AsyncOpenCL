#pragma once
#include <CL/opencl.h>

#include "Platform.h"
#include "Device.h"

namespace OpenCL
{
	public ref class Context
	{
	internal:
		cl_context _context;

	private:
		static cl_context CreateContextFromType();
		static cl_context CreateContext(cl_device_id id);

	public:
		Context(void);
		Context(Device^ device);
		~Context();

	};
}
