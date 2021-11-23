
#include "SDLWindow.class.hpp"
#include <stdexcept>
#include <string>

namespace notrealengine
{
	SDLWindow::SDLWindow( std::string name, std::vector<int> winsize )
	{
		SDL::Init();

		if (winsize.size() != 2)
			throw std::invalid_argument( "SDL Window constructor expects a size 2 vector" );

		windowHandle = SDL_CreateWindow(
				name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				winsize[0], winsize[1],
				SDL_WINDOW_OPENGL);
		if (windowHandle == nullptr)
			throw std::runtime_error( "Failed to create SDL Window : " + std::string(SDL_GetError()) );

		glHandle = SDL_GL_CreateContext(windowHandle);
		if (glHandle == nullptr)
		{
			SDL_DestroyWindow(windowHandle);
			throw std::runtime_error( "SDL failed to initalize GL context : " + std::string(SDL_GetError()) );
		}
	}

	SDLWindow::~SDLWindow( void )
	{
		SDL_GL_DeleteContext(glHandle);
		SDL_DestroyWindow(windowHandle);
	}

}
