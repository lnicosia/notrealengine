
#ifndef  __NOTREALENGINE_GL_HPP__
# define __NOTREALENGINE_GL_HPP__

# if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  include <windows.h>
#  include <glad/glad.h>
# else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/gl.h>
# endif

# include "GLException.class.hpp"
# include <filesystem>

# if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
# define GLCallThrow(func_name, ...) GLCallTemplate<decltype(func_name)>::Call("func_name", func_name __VA_OPT__, __VA_ARGS__)
# else
# define GLCallThrow(func_name, ...) GLCallTemplate<decltype(func_name)>::Call("func_name", func_name __VA_OPT__(,) __VA_ARGS__)
#endif

namespace notrealengine
{
	template<typename R, typename ... Args>
	struct GLCallTemplate;

	template<typename R, typename ... Args>
	struct GLCallTemplate<R (*)(Args...)>
	{
		static R Call(std::string func_name, R (* GLFunction)(Args ...), Args ... args)
		{
			GLenum error;
			error = glGetError();
			if (error != GL_NO_ERROR)
				throw GLException( "Previous OpenGL function failed and error wasn't checked", error );
			R ret = GLFunction(std::forward<Args>(args)...);
			error = glGetError();
			if (error != GL_NO_ERROR)
				throw GLException( "OpenGL function '" + func_name + "' failed.", error );
			return ret;
		};
	};

	template<typename ... Args>
	struct GLCallTemplate<void (*)(Args...)>
	{
		static void Call(std::string func_name, void (* GLFunction)(Args ...), Args ... args)
		{
			GLenum error;
			error = glGetError();
			if (error != GL_NO_ERROR)
				throw GLException( "Previous OpenGL function failed and error wasn't checked", error );
			GLFunction(std::forward<Args>(args)...);
			error = glGetError();
			if (error != GL_NO_ERROR)
				throw GLException( "OpenGL function '" + func_name + "' failed.", error );
		};
	};

	template<typename R, typename ... Args>
	struct GLCallTemplate<R(Args...)>
	{
		static R Call(std::string func_name, R GLFunction(Args ...), Args ... args)
		{
			GLenum error;
			error = glGetError();
			if (error != GL_NO_ERROR)
				throw GLException( "Previous OpenGL function failed and error wasn't checked", error );
			R ret = GLFunction(std::forward<Args>(args)...);
			error = glGetError();
			if (error != GL_NO_ERROR)
				throw GLException( "OpenGL function '" + func_name + "' failed.", error );
			return ret;
		};
	};

	template<typename ... Args>
	struct GLCallTemplate<void (Args...)>
	{
		static void Call(std::string func_name, void GLFunction(Args ...), Args ... args)
		{
			GLenum error;
			error = glGetError();
			if (error != GL_NO_ERROR)
				throw GLException( "Previous OpenGL function failed and error wasn't checked", error );
			error = glGetError();
			if (error != GL_NO_ERROR)
				throw GLException( "OpenGL function '" + func_name + "' failed.", error );
		};
	};
}

#endif

