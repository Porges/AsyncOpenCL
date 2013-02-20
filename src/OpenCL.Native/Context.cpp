#include "Context.h"
#include "Buffer.h"
#include "OpenCLException.h"

using namespace OpenCL;

Context::Context(Device^ device)
	: _context(CreateContext(device->_device_id))
{}

Context::Context(void)
	: _context(CreateContextFromType())
{}


cl_context Context::CreateContext(cl_device_id devId)
{
	cl_int errorCode;
	auto result = clCreateContext(nullptr, 1, &devId, nullptr, nullptr, &errorCode);
	if (errorCode == CL_SUCCESS)
	{
		return result;
	}
	else
	{
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));
	}

}

cl_context Context::CreateContextFromType()
{
	cl_int errCode;
	auto result = clCreateContextFromType(nullptr, CL_DEVICE_TYPE_CPU, nullptr, nullptr, &errCode);
	if (errCode == CL_SUCCESS)
	{
		return result;
	}
	else
	{
		throw gcnew OpenCLException(static_cast<ErrorCode>(errCode));
	}
}

Context::~Context()
{
	clReleaseContext(_context);
}

OpenCL::Buffer^ Context::CreateBuffer(size_t size)
{
	return gcnew Buffer(_context, CL_MEM_READ_WRITE, size, nullptr);
}