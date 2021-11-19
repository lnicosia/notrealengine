
#include "GLException.class.hpp"

namespace notrealengine
{
	GLException::GLException( std::string what_arg, GLenum gl_error ):
		std::runtime_error( ErrorString(gl_error) + " : " + what_arg ),
		glError(gl_error)
	{

		return ;
	}

	GLException::~GLException( void )
	{

		return ;
	}

	std::string GLException::ErrorString( GLenum error )
	{
		switch (error)
		{
			case GL_INVALID_ENUM:
				return std::string("GL_INVALID_ENUM");
			case GL_INVALID_VALUE:
				return std::string("GL_INVALID_VALUE");
			case GL_INVALID_OPERATION:
				return std::string("GL_INVALID_OPERATION");
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				return std::string("GL_INVALID_FRAMEBUFFER_OPERATION");
			case GL_OUT_OF_MEMORY:
				return std::string("GL_OUT_OF_MEMORY");
			default:
				return std::string("GL_UNKOWN_ERROR");
		}
	}
}
