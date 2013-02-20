#include "OpenCLException.h"

using namespace OpenCL;

OpenCLException::OpenCLException(OpenCL::ErrorCode ec)
	: _errorCode(ec)
	, Exception(ec.ToString())
{
}
