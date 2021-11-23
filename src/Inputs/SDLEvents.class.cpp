#include "Inputs/SDLEvents.class.hpp"

namespace notrealengine
{
	SDLEvents::SDLEvents(): e(), bindings(initBindings())
	{

	}

	SDLEvents::~SDLEvents()
	{

	}

	//	Init all bindings here
	std::vector<Binding>	SDLEvents::initBindings()
	{
		std::vector<Binding>	res;

		Binding	tmp = Binding(SDLK_ESCAPE, 0, false);
		tmp.onRelease = []() { return NRE_QUIT; };
		res.push_back(tmp);
		return res;
	}

	void	SDLEvents::processInputs()
	{
		std::vector<Binding>::const_iterator	it = bindings.begin();
		std::vector<Binding>::const_iterator	ite = bindings.end();
		while (it != ite)
		{
			Binding	binding = *it;
			switch (binding.getState())
			{
			case InputState::NRE_PRESS:
				if (binding.onPress != NULL)
					binding.onPress();
				break;
			case InputState::NRE_RELEASE:
				if (binding.onRelease != NULL)
					binding.onRelease();
				break;
			case InputState::NRE_PRESSED:
				if (binding.whenPressed != NULL)
					binding.whenPressed();
				break;
			case InputState::NRE_RELEASED:
				if (binding.whenReleased != NULL)
					binding.whenReleased();
				break;
			}
			it++;
		}
	}

	int	SDLEvents::updateInputsState()
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				return(NRE_QUIT);
				break;
			}
		}
		return (0);
	}

	int	SDLEvents::handle()
	{
		if (updateInputsState() == NRE_QUIT)
			return (NRE_QUIT);
		processInputs();
		return (0);
	}
}