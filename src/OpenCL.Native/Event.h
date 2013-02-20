#pragma once

#include <CL/cl.h>
#include "OpenCLException.h"

using namespace System::Runtime::CompilerServices;

namespace OpenCL
{
using namespace System;

public ref class Event : INotifyCompletion
{
internal:
	const cl_event _event;

	Event(cl_event);

public:
	property bool IsCompleted { bool get(); }
	virtual void OnCompleted(Action^ action);
	void GetResult();

	Event^ GetAwaiter() { return this; }

	~Event();
};

}