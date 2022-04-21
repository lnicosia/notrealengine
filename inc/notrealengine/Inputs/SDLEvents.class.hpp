#ifndef _SDLEVENTS_CLASS_H_
#define _SDLEVENTS_CLASS_H_

#include "Binding.class.hpp"
#include "SDL.h"
#include <vector>

#define	NRE_QUIT 1

namespace notrealengine
{
	struct SDLEvents
	{
	public:
		SDLEvents();
		~SDLEvents();

		/**	Run the whole input pipeline: update inputs state and launch
		**	functions accordingly
		*/
		int
			handle();

		/**	Update the state of every input to one of the 5 possible values:
		**	NRE_PRESS, NRE_RELEASE, NRE_PRESSED, NRE_RELEASED, NRE_HOVERED
		*/
		int
			updateInputsState();

		/**	According to the state of each input, launch binded functions
		*/
		void
			processInputs();

		std::vector<MouseBinding> mouseBindings;

		mft::vec2i	mousePos;
		mft::vec2i	mouseGlobalPos;
		InputState	mouseState;

		std::vector<Binding>	bindings;

		SDL_Event	e;

	};
}

#endif // !_SDLEVENTS_CLASS_H_
