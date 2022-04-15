#ifndef _SDLEVENTS_CLASS_H_
#define _SDLEVENTS_CLASS_H_

#include "Binding.class.hpp"
#include "SDL.h"
#include <vector>

#define	NRE_QUIT 1

namespace notrealengine
{
	class SDLEvents
	{
	public:
		SDLEvents();
		~SDLEvents();

		int
			handle();
		int
			updateInputsState();
		void
			processInputs();
		void
			AddBinding(const Binding binding);

	private:
		std::vector<Binding>	bindings;

		SDL_Event	e;

		/**	Init default bindings
		*/
		std::vector<Binding>
			initBindings();
	};
}

#endif // !_SDLEVENTS_CLASS_H_
