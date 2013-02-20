#include "Device.h"

using namespace OpenCL;

Device::Device(cl_device_id id)
	: _device_id(id)
{
}
