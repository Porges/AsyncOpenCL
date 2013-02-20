#include "Buffer.h"
#include "OpenCLException.h"


using namespace OpenCL;


OpenCL::Buffer::Buffer(Context^ context, size_t size)
	: MemoryObject(CreateBuffer(context->_context, CL_MEM_READ_WRITE, size, nullptr))
{}

cl_mem OpenCL::Buffer::CreateBuffer(cl_context context,
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

