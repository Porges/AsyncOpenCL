#pragma once

#include <CL/cl.h>



namespace OpenCL
{
	
public enum class ErrorCode
{
	DeviceNotFound = CL_DEVICE_NOT_FOUND,
	InvalidPlatform = CL_INVALID_PLATFORM,
	InvalidValue = CL_INVALID_VALUE,
	InvalidDevice = CL_INVALID_DEVICE,
	DeviceNotAvailable = CL_DEVICE_NOT_AVAILABLE,
	OutOfHostMemory = CL_OUT_OF_HOST_MEMORY,
	InvalidContext = CL_INVALID_CONTEXT,
	InvalidBufferSize = CL_INVALID_BUFFER_SIZE,
	InvalidHostPointer = CL_INVALID_HOST_PTR,
	MemObjectAllocationFailure = CL_MEM_OBJECT_ALLOCATION_FAILURE,
	InvalidEventWaitList = CL_INVALID_EVENT_WAIT_LIST,
	InvalidQueueProperties = CL_INVALID_QUEUE_PROPERTIES,
};


public ref class OpenCLException : public System::Exception
{
private:
	ErrorCode _errorCode;

public:
	OpenCLException(ErrorCode errorCode);

	property ErrorCode ErrorCode
	{
		OpenCL::ErrorCode get()
		{
			return _errorCode;
		}
	}
};

}