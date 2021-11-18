
#ifndef  __NRE_SDLWINDOW_CLASS_H__
# define __NRE_SDLWINDOW_CLASS_H__

# include "SDL.h"
# include "NRE_SDL.class.hpp"
# include <algorithm>
# include <stdexcept>

namespace notrealengine
{
	class SDLWindow {
		public:
			SDLWindow( std::string, std::vector<int> = {600, 800} )
				noexcept(false);
			~SDLWindow( void );

		private:
			SDL_Window * windowHandle;
			SDL_GLContext glHandle;
	};
}

#endif

