#ifndef _SDLEVENTS_CLASS_HPP_
# define _SDLEVENTS_CLASS_HPP_

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

		int	handle();
		int	updateInputsState();
		void	processInputs();

	private:
		std::vector<Binding>	bindings;

		SDL_Event	e;

		std::vector<Binding>
			initBindings();
	};
}

#endif