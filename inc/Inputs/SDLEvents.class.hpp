#ifndef _SDLEVENTS_CLASS_HPP_
# define _SDLEVENTS_CLASS_HPP_

// This define is used to circumvent an SDL bug that messes up clang-tidy
# define HAVE_STRING_H 1
#include "SDL.h"
#include "Binding.class.hpp"
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

		static std::vector<Binding>
			initBindings();
	};
} // namespace notrealengine

#endif
