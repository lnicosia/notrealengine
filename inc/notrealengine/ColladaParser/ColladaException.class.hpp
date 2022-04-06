#ifndef _COLLADA_EXCEPTION_CLASS_H_
#define	_COLLADA_EXCEPTION_CLASS_H_

#include <stdexcept>

namespace notrealengine
{
	class ColladaException: public std::runtime_error
	{
	public:
		ColladaException(std::string error);
		~ColladaException();

	private:

	};
}

#endif // !_COLLADA_EXCEPTION_CLASS_H_
