#include "CommandQueue.h"
#include "OpenCLException.h"

#include <vector>

using namespace OpenCL;
using namespace System::Runtime::InteropServices;

CommandQueue::CommandQueue(Context^ context, Device^ device)
	: _queue(CreateQueue(context->_context, device->_device_id))
{
}

CommandQueue::~CommandQueue()
{
	clReleaseCommandQueue(_queue);
}

cl_command_queue CommandQueue::CreateQueue(cl_context context, cl_device_id device)
{
	cl_int errorCode;
	auto result = clCreateCommandQueue(context, device, 0, &errorCode);
	if (errorCode != CL_SUCCESS)
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));
	return result;
}

void CommandQueue::Flush()
{
	auto errorCode = clFlush(_queue);
	if (errorCode != CL_SUCCESS)
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));
}

void CL_CALLBACK FreePinnedCallback(cl_event, cl_int, void* userDataPtr)
{
	auto handle = GCHandle::FromIntPtr(IntPtr(userDataPtr));
	handle.Free();
}

Event^ CommandQueue::EnqueueWrite(OpenCL::Buffer^ buffer, array<unsigned char>^ src, ... array<Event^>^ after)
{
	auto handle = GCHandle::Alloc(src, GCHandleType::Pinned);
	
	cl_event event;
	cl_int errorCode;
	if (after->Length > 0)
	{
		std::vector<cl_event> events;
		events.reserve(after->Length);
		for (int i = 0; i < after->Length; ++i)
			events.push_back(cl_event(after[i]->_event));
		errorCode = clEnqueueWriteBuffer(_queue, buffer->_mem, CL_FALSE, 0, src->Length, handle.AddrOfPinnedObject().ToPointer(), after->Length, events.data(), &event);
	}
	else
	{
		errorCode = clEnqueueWriteBuffer(_queue, buffer->_mem, CL_FALSE, 0, src->Length, handle.AddrOfPinnedObject().ToPointer(), 0, nullptr, &event);
	}
	if (errorCode != CL_SUCCESS)
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));

	errorCode = clSetEventCallback(event, CL_COMPLETE, &FreePinnedCallback, GCHandle::ToIntPtr(handle).ToPointer());
	if (errorCode != CL_SUCCESS)
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));

	return gcnew Event(event);
}

Event^ CommandQueue::EnqueueRead(OpenCL::Buffer^ buffer, array<unsigned char>^ dest, ... array<Event^>^ after)
{
	auto handle = GCHandle::Alloc(dest, GCHandleType::Pinned);
	
	cl_event event;
	cl_int errorCode;
	if (after->Length > 0)
	{
		std::vector<cl_event> events;
		events.reserve(after->Length);
		for (int i = 0; i < after->Length; ++i)
			events.push_back(cl_event(after[i]->_event));

		errorCode = clEnqueueReadBuffer(_queue, buffer->_mem, CL_FALSE, 0, dest->Length, handle.AddrOfPinnedObject().ToPointer(), after->Length, events.data(), &event);
	}
	else
	{
		errorCode = clEnqueueReadBuffer(_queue, buffer->_mem, CL_FALSE, 0, dest->Length, handle.AddrOfPinnedObject().ToPointer(), 0, nullptr, &event);
	}

	if (errorCode != CL_SUCCESS)
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));
	
	errorCode = clSetEventCallback(event, CL_COMPLETE, &FreePinnedCallback, GCHandle::ToIntPtr(handle).ToPointer());
	if (errorCode != CL_SUCCESS)
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));

	return gcnew Event(event);
}