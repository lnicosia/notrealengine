#include "Inputs/SDLEvents.class.hpp"
#include <iostream>

namespace notrealengine
{
	SDLEvents::SDLEvents(): e(), bindings(), mouseBindings(),
		mousePos(), mouseGlobalPos(), mouseState(InputState::NRE_RELEASED)
	{

	}

	SDLEvents::~SDLEvents()
	{
	}

	void	SDLEvents::processInputs()
	{
		for (auto& binding: this->bindings)
		{
			switch (binding.getState())
			{
				case InputState::NRE_PRESS:
				{
					if (binding.onPress)
						binding.onPress->execute();
					binding.setState(InputState::NRE_PRESSED);
					break;
				}
				case InputState::NRE_RELEASE:
				{
					if (binding.onRelease)
						binding.onRelease->execute();
					binding.setState(InputState::NRE_RELEASED);
					break;
				}
				case InputState::NRE_PRESSED:
				{
					if (binding.whenPressed)
						binding.whenPressed->execute();
					break;
				}
				case InputState::NRE_RELEASED:
				{
					if (binding.whenReleased)
						binding.whenReleased->execute();
					break;
				}
				default:
					break;
			}
		}
		for (auto& mouseBinding: this->mouseBindings)
		{
			switch (mouseBinding.getState())
			{
				case InputState::NRE_PRESS:
				{
					if (mouseBinding.onPress)
						mouseBinding.onPress->execute();
					mouseBinding.setState(InputState::NRE_PRESSED);
					break;
				}
				case InputState::NRE_RELEASE:
				{
					if (mouseBinding.onRelease)
						mouseBinding.onRelease->execute();
					mouseBinding.setState(InputState::NRE_RELEASED);
					break;
				}
				case InputState::NRE_PRESSED:
				{
					if (mouseBinding.whenPressed)
						mouseBinding.whenPressed->execute();
					break;
				}
				case InputState::NRE_RELEASED:
				{
					if (mouseBinding.whenReleased)
						mouseBinding.whenReleased->execute();
					break;
				}
				default:
					break;
			}
		}
	}

	int	SDLEvents::updateInputsState()
	{
		if (mouseState == InputState::NRE_PRESS)
			mouseState = InputState::NRE_PRESSED;
		if (mouseState == InputState::NRE_RELEASE)
			mouseState = InputState::NRE_RELEASED;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
				{
					return(NRE_QUIT);
					break;
				}
				case SDL_KEYDOWN:
				{
					for (auto& binding: this->bindings)
					{
						if (e.key.keysym.sym == binding.getKey1()
							|| e.key.keysym.sym == binding.getKey2())
						{
							if (binding.getState() == InputState::NRE_RELEASED)
								binding.setState(InputState::NRE_PRESS);
						}
					}
					break;
				}
				case SDL_KEYUP:
				{
					for (auto& binding: this->bindings)
					{
						if (e.key.keysym.sym == binding.getKey1()
							|| e.key.keysym.sym == binding.getKey2())
						{
							binding.setState(InputState::NRE_RELEASE);
						}
					}
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					for (auto& mouseBinding: this->mouseBindings)
					{
						if (e.button.button == mouseBinding.getKey1()
								|| e.button.button == mouseBinding.getKey2())
						{
							if (mouseBinding.getState() == InputState::NRE_RELEASED)
							{
								mouseBinding.setState(InputState::NRE_PRESS);
								mouseBinding.start = this->mousePos;
							}
						}
					}
					if (e.button.button == SDL_BUTTON_LEFT
						&& mouseState == InputState::NRE_RELEASED)
							mouseState = InputState::NRE_PRESS;
					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					for (auto& mouseBinding: this->mouseBindings)
					{
						if (e.button.button == mouseBinding.getKey1()
								|| e.button.button == mouseBinding.getKey2())
						{
							if (mouseBinding.getState() == InputState::NRE_PRESSED)
							{
								mouseBinding.setState(InputState::NRE_RELEASE);
							}
						}
					}
					if (e.button.button == SDL_BUTTON_LEFT
						&& mouseState == InputState::NRE_PRESSED)
					{
						mouseState = InputState::NRE_RELEASE;
					}
					break;
				}
				default:
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
