
#ifndef  __NRE_SDL_CLASS_H__
# define __NRE_SDL_CLASS_H__

# include "SDL.h"

namespace notrealengine
{

	class SDL {

	public:
		static void Init( void ) noexcept(false);
		static void Quit( void );

	private:
		SDL( void ) {};
		static void InitOpenGLContext( void ) noexcept(false);

		static bool initialized;
	};
}

#endif

