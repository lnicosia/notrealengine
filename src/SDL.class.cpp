
#include "SDL.class.hpp"

#include <cstdlib>
#include <iostream>

namespace notrealengine
{
	void SDL::Init( )
	{
		if (!initialized) {
			if (SDL_InitSubSystem(SDL_INIT_EVERYTHING) != 0) {
				throw std::runtime_error( "Couldn't initialize SDL : \n" + std::string(SDL_GetError()) );
			}

			SetupGLOptions();

			if (std::atexit(Quit) != 0)
			{
				SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
				throw std::runtime_error( "Failed to setup SDL cleanup function\n" );
			}

			initialized = true;
		}
	}

	void SDL::SetupGLOptions()
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	void SDL::Quit( )
	{
		if (initialized) {
			SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
			initialized = false;
		}
	}

	bool SDL::initialized = false;
} // namespace notrealengine
