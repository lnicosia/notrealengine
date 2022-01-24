
#include "SDLWindow.class.hpp"
#include <stdexcept>
#include <string>

namespace notrealengine
{
	SDLWindow::SDLWindow( const std::string& name, std::pair<int, int> winsize ) : windowHandle(SDL_CreateWindow(
				name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				winsize.first, winsize.second,
				SDL_WINDOW_OPENGL))
	{
		SDL::Init();

		
		if (windowHandle == nullptr) {
			throw std::runtime_error( "Failed to create SDL Window : " + std::string(SDL_GetError()) );
}

		glHandle = SDL_GL_CreateContext(windowHandle);
		if (glHandle == nullptr)
		{
			SDL_DestroyWindow(windowHandle);
			throw std::runtime_error( "SDL failed to initalize GL context : " + std::string(SDL_GetError()) );
		}
	}

	SDLWindow::~SDLWindow( )
	{
		SDL_GL_DeleteContext(glHandle);
		SDL_DestroyWindow(windowHandle);
	}

	SDL_Window* SDLWindow::getWindow()
	{
		return windowHandle;
	}

	SDL_GLContext SDLWindow::getContext()
	{
		return glHandle;
	}
} // namespace notrealengine
