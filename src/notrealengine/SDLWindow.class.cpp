
#include "SDLWindow.class.hpp"
#include <stdexcept>
#include <string>
#include <iostream>

# if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#  include <windows.h>
#  include <glad/glad.h>
# endif

namespace notrealengine
{
	SDLWindow::SDLWindow( const std::string& name, std::pair<int, int> winsize )
	{
		SDL::Init();

		windowHandle = SDL_CreateWindow(
				name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				winsize.first, winsize.second,
				SDL_WINDOW_OPENGL);
		if (windowHandle == nullptr)
			throw std::runtime_error( "Failed to create SDL Window : " + std::string(SDL_GetError()) );

		glHandle = SDL_GL_CreateContext(windowHandle);
		if (glHandle == nullptr)
		{
			SDL_DestroyWindow(windowHandle);
			throw std::runtime_error( "SDL failed to initalize GL context : " + std::string(SDL_GetError()) );
		}
		loadGLfunctions();
	}

	SDLWindow::~SDLWindow( void )
	{
		SDL_GL_DeleteContext(glHandle);
		SDL_DestroyWindow(windowHandle);
	}

	void	SDLWindow::loadGLfunctions(void)
	{
# if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		gladLoadGLLoader(SDL_GL_GetProcAddress);
# endif
	}

	SDL_Window* SDLWindow::getWindow()
	{
		return windowHandle;
	}

	SDL_GLContext SDLWindow::getContext()
	{
		return glHandle;
	}
}
