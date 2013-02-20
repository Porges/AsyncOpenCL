#include "Buffer.h"
#include "OpenCLException.h"

using namespace OpenCL;

Buffer::Buffer(cl_context context,
 	cl_mem_flags flags,
 	size_t size,
 	void *host_ptr) :
	MemoryObject(CreateBuffer(context, flags, size, host_ptr))
{

}

cl_mem Buffer::CreateBuffer(cl_context context,
 	cl_mem_flags flags,
 	size_t size,
 	void *host_ptr)
{
	cl_int errCode;
	auto result = clCreateBuffer(context, flags, size, host_ptr, &errCode);
	if (errCode == CL_SUCCESS)
	{
		return result;
	}
	else
	{
		throw gcnew OpenCLException(static_cast<ErrorCode>(errCode));
	}
}