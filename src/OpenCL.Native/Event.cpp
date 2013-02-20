#include "Event.h"

using namespace OpenCL;
using namespace System::Runtime::InteropServices;

Event::Event(cl_event ev)
	: _event(ev)
{
}

bool Event::IsCompleted::get()
{
	cl_int status;
	auto errorCode = clGetEventInfo(_event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(status), &status, nullptr);
	if (errorCode != CL_SUCCESS)
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));

	return status < 0 || status == CL_COMPLETE;
}

void CL_CALLBACK ActivateCallback(cl_event ev, cl_int status, void* userData)
{
	auto handle = GCHandle::FromIntPtr(IntPtr(userData));
	
	Action^ action = (Action^)handle.Target;
	handle.Free();
	action->DynamicInvoke();
}

void Event::OnCompleted(Action^ action)
{
	auto handle = GCHandle::Alloc(action, GCHandleType::Normal);
	auto errorCode = clSetEventCallback(_event, CL_COMPLETE, &ActivateCallback, GCHandle::ToIntPtr(handle).ToPointer());
	if (errorCode != CL_SUCCESS)
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));
}

void Event::GetResult()
{
	auto ev = _event;
	auto errorCode = clWaitForEvents(1, &ev);
	if (errorCode != CL_SUCCESS)
		throw gcnew OpenCLException(static_cast<ErrorCode>(errorCode));
}

Event::~Event()
{
	clReleaseEvent(_event);
}