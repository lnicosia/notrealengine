
#ifndef  __SDL_CLASS_H__
# define __SDL_CLASS_H__

// This define is used to circumvent an SDL bug that messes up clang-tidy
# define HAVE_STRING_H 1
# include "SDL.h"

namespace notrealengine
{

	class SDL {

	public:
		static void Init( );
		static void Quit( );

	private:
		SDL( ) = default;;
		static void SetupGLOptions( );

		static bool initialized;
	};
} // namespace notrealengine

#endif

