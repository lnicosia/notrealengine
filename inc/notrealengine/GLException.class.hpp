
#ifndef  __GLEXCEPTION_HPP__
# define __GLEXCEPTION_HPP__

# include <filesystem>
# if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  include <windows.h>
#  include <glad/glad.h>
# else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/gl.h>
# endif

# include <stdexcept>

namespace notrealengine
{
	class GLException : public std::runtime_error {
	public:
		GLException( std::string what_arg, GLenum gl_error );
		~GLException( void );

		const GLenum glError;
		static std::string ErrorString( GLenum error );
	};
}

#endif

