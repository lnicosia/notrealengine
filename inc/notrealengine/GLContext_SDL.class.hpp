
#ifndef  __GLCONTEXT_SDL_CLASS_H__
# define __GLCONTEXT_SDL_CLASS_H__

# include "SDL.h"
# include "GLContext.class.hpp"

namespace notrealengine
{
	class GLContext_SDL : public GLContext {

	public:
		GLContext_SDL( SDL_GLContext, SDL_Window * );
		~GLContext_SDL( void );

		void
			swapWindow(void);
		void
			makeCurrent( void ) override;
		bool
			isCurrent( void ) override;

	private:
		SDL_Window *
			windowHandle;
		SDL_GLContext
			glHandle;
	};
}

#endif
