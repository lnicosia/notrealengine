
#include "GLContext_SDL.class.hpp"

namespace notrealengine
{
	GLContext_SDL::GLContext_SDL( SDL_GLContext glHandle, SDL_Window * windowHandle ):
		glHandle(glHandle), windowHandle(windowHandle)
	{

			}

	GLContext_SDL::~GLContext_SDL( )
	{

			}

	void GLContext_SDL::makeCurrent( )
	{
		SDL_GL_MakeCurrent(windowHandle, glHandle);
		CurrentContext = (long)glHandle;
	}

	void GLContext_SDL::swapWindow()
	{
		SDL_GL_SwapWindow(windowHandle);
	}

	bool GLContext_SDL::isCurrent( )
	{
		return (CurrentContext == (long)glHandle);
	}
} // namespace notrealengine
