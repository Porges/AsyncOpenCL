#pragma once
#include <CL/cl.h>

namespace OpenCL
{

public ref class MemoryObject abstract
{
internal:
	const cl_mem _mem;

protected:
	MemoryObject(cl_mem mem);

public:
	~MemoryObject();
};

}