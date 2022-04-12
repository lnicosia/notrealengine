
#include "GLContext_SDL.class.hpp"
#include <iostream>
namespace notrealengine
{
	GLContext_SDL::GLContext_SDL( SDL_GLContext glHandle, SDL_Window * windowHandle ):
		glHandle(glHandle), windowHandle(windowHandle)
	{
		makeCurrent();
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

	void GLContext_SDL::swapWindow(void)
	{
		SDL_GL_SwapWindow(windowHandle);
	}

	bool GLContext_SDL::isCurrent( void )
	{
		return (CurrentContext == (long)glHandle);
	}
}
