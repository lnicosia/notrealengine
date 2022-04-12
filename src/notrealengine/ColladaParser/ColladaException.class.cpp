#include "ColladaParser/ColladaException.class.hpp"

namespace notrealengine
{
	ColladaException::ColladaException(std::string error):
		std::runtime_error(error)
	{

	}

	ColladaException::~ColladaException()
	{

	}
}