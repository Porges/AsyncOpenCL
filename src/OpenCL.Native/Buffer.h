#pragma once

#include <CL/opencl.h>
#include "MemoryObject.h"

namespace OpenCL
{

	public ref class Buffer : MemoryObject
	{
	private:
		static cl_mem CreateBuffer(cl_context context,
							cl_mem_flags flags,
							size_t size,
							void *host_ptr);
	internal:
		Buffer(cl_context context,
			cl_mem_flags flags,
			size_t size,
			void *host_ptr);

	}; 


}