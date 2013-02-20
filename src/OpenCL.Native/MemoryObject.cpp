#include "MemoryObject.h"

using namespace OpenCL;

MemoryObject::MemoryObject(cl_mem mem) : _mem(mem)
{
}

MemoryObject::~MemoryObject()
{
	clReleaseMemObject(_mem);
}