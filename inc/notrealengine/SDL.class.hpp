
#ifndef  __SDL_CLASS_H__
# define __SDL_CLASS_H__

// This define is used to circumvent an SDL bug that messes up clang-tidy
# define HAVE_STRING_H 1
# include "SDL.h"

namespace notrealengine
{

	class SDL {

	public:
		static void Init( void );
		static void Quit( void );

	private:
		SDL( void ) {};
		static void SetupGLOptions( void );

		static bool initialized;
	};
}

#endif

