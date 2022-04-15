#include "Inputs/SDLEvents.class.hpp"
#include <iostream>

namespace notrealengine
{
	SDLEvents::SDLEvents(): e(), bindings(initBindings())
	{

	}

	SDLEvents::~SDLEvents()
	{
		for (auto binding: this->bindings)
		{
			if (binding.onRelease)
				delete binding.onRelease;
			if (binding.onPress)
				delete binding.onPress;
			if (binding.whenPressed)
				delete binding.whenPressed;
			if (binding.whenReleased)
				delete binding.whenReleased;
		}
	}

	//	Init default bindings
	std::vector<Binding>	SDLEvents::initBindings()
	{
		std::vector<Binding>	res;

		return res;
	}

	void	SDLEvents::processInputs()
	{
		std::vector<Binding>::iterator	it = this->bindings.begin();
		std::vector<Binding>::iterator	ite = this->bindings.end();
		while (it != ite)
		{
			Binding binding = *it;
			switch (binding.getState())
			{
			case InputState::NRE_PRESS:
			{
				if (it->onPress)
					it->onPress->execute();
				it->setState(InputState::NRE_PRESSED);
				break;
			}
			case InputState::NRE_RELEASE:
			{
				if (it->onRelease)
					it->onRelease->execute();
				it->setState(InputState::NRE_RELEASED);
				break;
			}
			case InputState::NRE_PRESSED:
			{
				if (it->whenPressed)
					it->whenPressed->execute();
				break;
			}
			case InputState::NRE_RELEASED:
			{
				if (it->whenReleased)
					it->whenReleased->execute();
				break;
			}
			default:
				break;
			}
			it++;
		}
	}

	int	SDLEvents::updateInputsState()
	{
		std::vector<Binding>::iterator	it;
		std::vector<Binding>::iterator	ite;
		while (SDL_PollEvent(&e))
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
						if ((*it).getState() == InputState::NRE_RELEASED)
							(*it).setState(InputState::NRE_PRESS);
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
		if (updateInputsState() == NRE_QUIT)
			return (NRE_QUIT);
		processInputs();
		return (0);
	}

	void SDLEvents::AddBinding(const Binding b)
	{
		this->bindings.push_back(b);
	}
}
