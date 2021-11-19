
#include "GLContext_SDL.class.hpp"

namespace notrealengine
{
	GLContext_SDL::GLContext_SDL( SDL_GLContext glHandle, SDL_Window * windowHandle ):
		glHandle(glHandle), windowHandle(windowHandle)
	{

		return ;
	}

	GLContext_SDL::~GLContext_SDL( void )
	{

		return ;
	}

	void GLContext_SDL::makeCurrent( void )
	{
		SDL_GL_MakeCurrent(windowHandle, glHandle);
		CurrentContext = (long)glHandle;
	}

	bool GLContext_SDL::isCurrent( void )
	{
		return (CurrentContext == (long)glHandle);
	}
}
