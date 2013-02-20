#pragma once

#include <CL/opencl.h>
#include "MemoryObject.h"
#include "Context.h"

namespace OpenCL
{

	public ref class Buffer : MemoryObject
	{
	private:
		cl_mem CreateBuffer(cl_context context,
			cl_mem_flags flags,
			size_t size,
			void *host_ptr);

	public:
		Buffer(Context^ context, size_t size);
	}; 


}