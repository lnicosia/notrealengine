
#ifndef  __GLCONTEXT_SDL_CLASS_H__
# define __GLCONTEXT_SDL_CLASS_H__

// This define is used to circumvent an SDL bug that messes up clang-tidy
# define HAVE_STRING_H 1
# include "SDL.h"
# include "GLContext.class.hpp"

namespace notrealengine
{
	class GLContext_SDL : public GLContext {

	public:
		GLContext_SDL( SDL_GLContext, SDL_Window * );
		~GLContext_SDL( );

		void
			swapWindow();
		void
			makeCurrent( ) override;
		bool
			isCurrent( ) override;

	private:
		SDL_Window *
			windowHandle;
		SDL_GLContext
			glHandle;
	};
} // namespace notrealengine

#endif
