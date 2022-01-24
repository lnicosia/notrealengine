#include "Inputs/SDLEvents.class.hpp"
#include <iostream>

namespace notrealengine
{
	SDLEvents::SDLEvents(): e(), bindings(initBindings())
	{

	}

	SDLEvents::~SDLEvents()
	= default;

	//	Init all bindings here
	std::vector<Binding>	SDLEvents::initBindings()
	{
		std::vector<Binding>	res;

		//	Test stuff here

		Binding	tmp = Binding("Escape", SDLK_ESCAPE, 0, false);
		tmp.onRelease.push_back([]() { std::exit(0); return 1; });
		res.push_back(tmp);
		/*std::cout << "Init bindings" << std::endl;
		
		Binding	tmp2 = Binding("K key", SDLK_k, 0, false);
		tmp2.onPress.push_back([]() { std::cout << "COUCOU" << std::endl; return 0; });
		res.push_back(tmp2);*/
		return res;
	}

	void	SDLEvents::processInputs()
	{
		auto	it = bindings.begin();
		auto	ite = bindings.end();
		while (it != ite)
		{
			Binding	binding = *it;
			std::vector<int (*)()>::const_iterator f;
			std::vector<int (*)()>::const_iterator fe;
			switch (binding.getState())
			{
			case InputState::NRE_PRESS:
				f = binding.onPress.begin();
				fe = binding.onPress.end();
				(*it).setState(InputState::NRE_PRESSED);
				break;
			case InputState::NRE_RELEASE:
				f = binding.onRelease.begin();
				fe = binding.onRelease.end();
				(*it).setState(InputState::NRE_RELEASED);
				break;
			case InputState::NRE_PRESSED:
				f = binding.whenPressed.begin();
				fe = binding.whenPressed.end();
				break;
			case InputState::NRE_RELEASED:
				f = binding.whenReleased.begin();
				fe = binding.whenReleased.end();
				break;
			}
			while (f != fe)
			{
				(*f)();
				f++;
			}
			it++;
		}
	}

	int	SDLEvents::updateInputsState()
	{
		std::vector<Binding>::iterator	it;
		std::vector<Binding>::iterator	ite;
		while (SDL_PollEvent(&e) != 0)
		{
			it = bindings.begin();
			ite = bindings.end();
			switch (e.type)
			{
			case SDL_QUIT:
				return(NRE_QUIT);
				break;
			case SDL_KEYDOWN:
				while (it != ite)
				{
					if (e.key.keysym.sym == (*it).getKey1()
						|| e.key.keysym.sym == (*it).getKey2())
					{
						if ((*it).getState() == InputState::NRE_RELEASED) {
							(*it).setState(InputState::NRE_PRESS);
}
					}
					it++;
				}
				break;
			case SDL_KEYUP:
				while (it != ite)
				{
					if (e.key.keysym.sym == (*it).getKey1()
						|| e.key.keysym.sym == (*it).getKey2())
					{
						(*it).setState(InputState::NRE_RELEASE);
					}
					it++;
				}
				break;
			}
		}
		return (0);
	}

	int	SDLEvents::handle()
	{
		if (updateInputsState() == NRE_QUIT) {
			return (NRE_QUIT);
}
		processInputs();
		return (0);
	}
} // namespace notrealengine