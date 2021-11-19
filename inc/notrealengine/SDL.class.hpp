
#ifndef  __SDL_CLASS_H__
# define __SDL_CLASS_H__

# include "SDL.h"

namespace notrealengine
{

	class SDL {

	public:
		static void Init( void );
		static void Quit( void );

	private:
		SDL( void ) {};
		static void InitOpenGLContext( void );

		static bool initialized;
	};
}

#endif

