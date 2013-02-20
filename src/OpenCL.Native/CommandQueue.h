#pragma once

#include <CL/cl.h>
#include "Event.h"
#include "Context.h"
#include "Device.h"

namespace OpenCL
{

public ref class CommandQueue
{
private:
	cl_command_queue _queue;

	static cl_command_queue CreateQueue(cl_context ctx, cl_device_id device);

public:
	CommandQueue(Context^ context, Device^ device);

	~CommandQueue();

	Event^ EnqueueWrite(Buffer^ buffer, array<unsigned char>^ source, ... array<Event^>^ after);
	Event^ EnqueueRead(Buffer^ buffer, array<unsigned char>^ dest, ... array<Event^>^ after);
	void Flush();
//	Event^ EnqueueEvent();
};

}

