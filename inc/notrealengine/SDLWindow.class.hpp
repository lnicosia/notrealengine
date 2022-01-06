
#ifndef  __SDLWINDOW_CLASS_H__
# define __SDLWINDOW_CLASS_H__

// This define is used to circumvent an SDL bug that messes up clang-tidy
# define HAVE_STRING_H 1
# include "SDL.h"
# include "SDL.class.hpp"
# include <algorithm>
# include <stdexcept>
# include <vector>

namespace notrealengine
{
	class SDLWindow {
		public:
			SDLWindow( std::string, std::pair<int, int> = {600, 800} );
			~SDLWindow( void );

			SDL_Window* getWindow();
			SDL_GLContext getContext();

		private:
			SDL_Window * windowHandle;
			SDL_GLContext glHandle;
	};
}

#endif

